#include "tgcloud/gallery/media.hpp"
namespace tgcloud::gallery {void Catalog::add(Media m){items_.push_back(std::move(m));}std::vector<Media> Catalog::list(size_t l,size_t o)const{std::vector<Media> r;for(size_t i=o;i<items_.size()&&r.size()<l;i++)r.push_back(items_[i]);return r;}}
