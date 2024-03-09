#include <unordered_map>
#include "../library/stb_image.h"
#include "../library/tnl_util.h"
#include "dxlib_ext_texture.h"
#include <WICTextureLoader.h>

namespace dxe {

	std::unordered_map<size_t, Shared<dxe::Texture>> Texture::managed_map_;

	std::shared_ptr<Texture> Texture::CreateFromMemory(char* file_data, uint32_t data_size, const std::string& regist_key_) {

		size_t hash = (regist_key_.empty()) ? std::hash<char*>()(file_data) : std::hash<std::string>()(regist_key_);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
		tex->graph_hdl_ = CreateGraphFromMem(file_data, data_size);
		GetGraphSize(tex->graph_hdl_, (int*)(&tex->width_), (int*)(&tex->height_));
		if (!regist_key_.empty()) {
			tex->file_name_ = regist_key_;
			tex->file_path_ = regist_key_;
		}
		tex->data_size_ = data_size;
		tex->file_data_ = new char[tex->data_size_];
		memcpy(tex->file_data_, file_data, tex->data_size_);
		managed_map_.insert(std::make_pair(hash, tex));
		return tex;
	}

	std::shared_ptr<Texture> Texture::CreateFromColorBufferRGBA8(unsigned char* color_buffer_rgba8, uint32_t width, uint32_t height, const std::string& regist_key) {
		size_t hash = std::hash<std::string>()(regist_key);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
		uint32_t data_size = 0;
		auto buff_tga = tnl::CreateFormatTga32((unsigned char*)color_buffer_rgba8, width, height, &data_size);

		char* file_data = new char[data_size];
		memcpy(file_data, buff_tga.get(), data_size);

		tex->graph_hdl_ = CreateGraphFromMem(buff_tga.get(), data_size);
		tex->width_ = width;
		tex->height_ = height;
		tex->file_data_ = file_data;
		tex->data_size_ = data_size;

		managed_map_.insert(std::make_pair(hash, tex));

		return tex;
	}


	std::shared_ptr<Texture> Texture::CreateFromFile(const std::string& file_path)
	{
		size_t hash = std::hash<std::string>()(file_path);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>( new Texture() );
		tex->file_path_ = file_path;
		std::tuple t = tnl::DetachmentFilePath(file_path);
		tex->file_name_ = std::get<1>(t) + "." + std::get<2>(t);

		tex->data_size_ = (uint32_t)tnl::GetFileSize(file_path.c_str());

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "rb");
		if (fp) {
			tex->file_data_ = new char[tex->data_size_];
			fread(tex->file_data_, tex->data_size_, 1, fp);
			fclose(fp);
		}

		tex->graph_hdl_ = CreateGraphFromMem(tex->file_data_, tex->data_size_);
		GetGraphSize(tex->graph_hdl_, (int*)(&tex->width_), (int*)(&tex->height_));

		managed_map_.insert(std::make_pair( hash, tex ));

		return tex;
	}

	std::shared_ptr<Texture> Texture::CreateFromDxLibGraphHdl(int dxlib_graph_hdl)
	{
		size_t hash = std::hash<int>()(dxlib_graph_hdl);
		auto it = managed_map_.find(hash);
		if (it != managed_map_.end()) {
			return it->second;
		}

		std::shared_ptr<Texture> tex = std::shared_ptr<Texture>(new Texture());
		tex->graph_hdl_ = dxlib_graph_hdl;
		int w, h;
		GetGraphSize(tex->graph_hdl_, &w, &h);
		tex->width_ = w;
		tex->height_ = h;

		managed_map_.insert(std::make_pair(hash, tex));
		return tex;
	}


	std::vector<std::shared_ptr<Texture>> Texture::CreateFromFileDiv(const std::string& file_path, uint32_t frame_w, uint32_t frame_h) {
		std::vector<std::shared_ptr<Texture>> out;
		out.resize(frame_w * frame_h);

		auto col_tpl = tnl::LoadGraphicColorBuffer(file_path.c_str());
		if (std::get<0>(col_tpl) == nullptr) return std::vector<std::shared_ptr<Texture>>() ;

		uint32_t width = std::get<1>(col_tpl);
		uint32_t height = std::get<2>(col_tpl);
		uint32_t fr_pc_w = width / frame_w;
		uint32_t fr_pc_h = height / frame_h;

		unsigned char* root = (unsigned char*)std::get<0>(col_tpl).get();

		for (uint32_t i = 0; i < frame_h; ++i) {
			unsigned char* gp = root + ( i * width * 4 ) * fr_pc_h ;

			for (uint32_t k = 0; k < frame_w; ++k) {

				uint32_t data_size = fr_pc_w * fr_pc_h * 4;
				unsigned char* col_buff = new unsigned char[data_size];

				unsigned char* p = gp;
				unsigned char* cp = col_buff;
				for (int n = 0; n < fr_pc_h; ++n) {
					memcpy(cp, p, fr_pc_w * 4);
					p += width * 4;
					cp += fr_pc_w * 4;
				}
				gp += fr_pc_w * 4 ;

				std::shared_ptr<Texture> tex = CreateFromColorBufferRGBA8((unsigned char*)col_buff, fr_pc_w, fr_pc_h, file_path + std::to_string((i * frame_w) + k));
				out[( i * frame_w ) + k] = tex;
				delete[] col_buff;
			}
		}

		return out;
	}


	void Texture::DestroyUnReferenceTextures() {
		auto it = managed_map_.begin();
		while (it != managed_map_.end()) {
			if (1 == it->second.use_count()) {
				it = managed_map_.erase(it);
				continue;
			}
			it++;
		}
	}

}

