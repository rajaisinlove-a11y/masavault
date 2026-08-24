#include "http_server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

namespace tgcloud::desktop {
namespace {

std::string mime_for(const std::string& path) {
    if (path.ends_with(".html")) return "text/html; charset=utf-8";
    if (path.ends_with(".css")) return "text/css; charset=utf-8";
    if (path.ends_with(".js")) return "application/javascript; charset=utf-8";
    if (path.ends_with(".svg")) return "image/svg+xml";
    return "application/octet-stream";
}

void send_all(int fd, const std::string& s) {
    size_t off = 0;
    while (off < s.size()) {
        auto n = ::send(fd, s.data() + off, s.size() - off, 0);
        if (n <= 0) return;
        off += static_cast<size_t>(n);
    }
}

void respond(int fd, int code, const std::string& type, const std::string& body) {
    std::ostringstream h;
    h << "HTTP/1.1 " << code << " OK\r\n"
      << "Content-Type: " << type << "\r\n"
      << "Content-Length: " << body.size() << "\r\n"
      << "Access-Control-Allow-Origin: *\r\n"
      << "Access-Control-Allow-Headers: Content-Type, X-Filename\r\n"
      << "Connection: close\r\n\r\n";
    send_all(fd, h.str());
    send_all(fd, body);
}

std::string url_decode(std::string s) {
    std::string o;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '%' && i + 2 < s.size()) {
            o += static_cast<char>(std::strtol(s.substr(i + 1, 2).c_str(), nullptr, 16));
            i += 2;
        } else if (s[i] == '+')
            o += ' ';
        else
            o += s[i];
    }
    return o;
}

std::string form_field(const std::string& body, const std::string& key) {
    auto pat = key + "=";
    auto p = body.find(pat);
    if (p == std::string::npos) return {};
    p += pat.size();
    auto e = body.find('&', p);
    return url_decode(body.substr(p, e == std::string::npos ? std::string::npos : e - p));
}

void handle(int fd, const std::string& www, Session& session) {
    std::string req;
    char buf[8192];
    while (req.find("\r\n\r\n") == std::string::npos) {
        auto n = ::recv(fd, buf, sizeof buf, 0);
        if (n <= 0) return;
        req.append(buf, static_cast<size_t>(n));
        if (req.size() > 32 * 1024 * 1024) return;
    }
    auto hdr_end = req.find("\r\n\r\n");
    auto headers = req.substr(0, hdr_end);
    auto body = req.substr(hdr_end + 4);
    std::istringstream hs(headers);
    std::string method, path, ver;
    hs >> method >> path >> ver;
    size_t content_len = 0;
    std::string filename = "upload.bin";
    std::istringstream hl(headers);
    std::string line;
    while (std::getline(hl, line)) {
        if (line.rfind("Content-Length:", 0) == 0 || line.rfind("content-length:", 0) == 0)
            content_len = static_cast<size_t>(std::stoul(line.substr(line.find(':') + 1)));
        if (line.rfind("X-Filename:", 0) == 0 || line.rfind("x-filename:", 0) == 0) {
            filename = line.substr(line.find(':') + 1);
            while (!filename.empty() && (filename.front() == ' ' || filename.front() == '\r')) filename.erase(filename.begin());
            while (!filename.empty() && (filename.back() == '\r' || filename.back() == ' ')) filename.pop_back();
        }
    }
    while (body.size() < content_len) {
        auto n = ::recv(fd, buf, sizeof buf, 0);
        if (n <= 0) break;
        body.append(buf, static_cast<size_t>(n));
    }
    auto q = path.find('?');
    auto route = q == std::string::npos ? path : path.substr(0, q);

    if (method == "OPTIONS") {
        respond(fd, 204, "text/plain", "");
        return;
    }
    if (method == "GET" && (route == "/" || route == "/index.html")) {
        std::ifstream in(www + "/index.html");
        std::string html((std::istreambuf_iterator<char>(in)), {});
        if (html.empty()) html = "<h1>TG Cloud Desktop</h1><p>www files missing</p>";
        respond(fd, 200, "text/html; charset=utf-8", html);
        return;
    }
    if (method == "GET" && route == "/api/status") {
        respond(fd, 200, "application/json", session.status_json());
        return;
    }
    if (method == "GET" && route == "/api/files") {
        respond(fd, 200, "application/json", session.files_json());
        return;
    }
    if (method == "GET" && route == "/api/transfers") {
        respond(fd, 200, "application/json", session.transfers_json());
        return;
    }
    if (method == "GET" && route == "/api/diagnostics") {
        respond(fd, 200, "text/plain; charset=utf-8", session.diagnostics_text());
        return;
    }
    if (method == "POST" && route == "/api/setup") {
        auto channel = form_field(body, "channel");
        std::vector<std::string> tokens;
        for (int i = 1; i <= 8; ++i) {
            auto t = form_field(body, "bot" + std::to_string(i));
            if (!t.empty()) tokens.push_back(t);
        }
        respond(fd, 200, "application/json", session.setup(channel, tokens));
        return;
    }
    if (method == "POST" && route == "/api/upload") {
        if (filename.empty()) filename = "upload.bin";
        respond(fd, 200, "application/json", session.upload_bytes(filename, body));
        return;
    }
    if (method == "POST" && route == "/api/share") {
        auto pw = form_field(body, "password");
        auto ids_raw = form_field(body, "ids");
        std::vector<std::string> ids;
        std::stringstream ss(ids_raw);
        std::string id;
        while (std::getline(ss, id, ','))
            if (!id.empty()) ids.push_back(id);
        respond(fd, 200, "application/json", session.share_selected(ids, pw.empty() ? "share" : pw));
        return;
    }
    if (method == "GET") {
        auto file = www + route;
        if (file.find("..") != std::string::npos) {
            respond(fd, 404, "text/plain", "not found");
            return;
        }
        std::ifstream in(file, std::ios::binary);
        if (!in) {
            respond(fd, 404, "text/plain", "not found");
            return;
        }
        std::string data((std::istreambuf_iterator<char>(in)), {});
        respond(fd, 200, mime_for(file), data);
        return;
    }
    respond(fd, 404, "text/plain", "not found");
}

}  // namespace

void serve_forever(const std::string& host, int port, const std::string& www, Session& session,
                   std::atomic<bool>* running) {
    int s = ::socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(port));
    addr.sin_addr.s_addr = host == "0.0.0.0" ? INADDR_ANY : inet_addr(host.c_str());
    if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof addr) != 0) throw std::runtime_error("bind failed");
    listen(s, 32);
    while (!running || running->load()) {
        sockaddr_in cli{};
        socklen_t n = sizeof cli;
        int c = accept(s, reinterpret_cast<sockaddr*>(&cli), &n);
        if (c < 0) continue;
        std::thread([c, www, &session] {
            handle(c, www, session);
            close(c);
        }).detach();
    }
    close(s);
}

}  // namespace tgcloud::desktop
