#include "../core/deng_core.h"

namespace dengUI {

    PixelCollision::PixelCollision(deng::Window *p_window, std::vector<deng::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data) {
        this->m_p_window = p_window;
        this->m_p_vertices_data = p_vertices_data;
        this->m_p_indices_data = p_indices_data;
        this->m_objects_pixel_data.resize(0);
    }

    void PixelCollision::getRowPixelBoundaries(const size_t &vertical_index, deng::vec2<deng::vec2<double>> &boundaries, const char *p_description) {
        size_t local_pixel_group_index;
        deng::vec2<deng::vec2<double>> local_sorting_buffer;

        for(size_t i = 0; i < this->m_objects_pixel_data.size(); i++) 
            if(this->m_objects_pixel_data[i].object_description == p_description) local_pixel_group_index = i;
        

        // sort pixels in growing order
        for(size_t i = 0; i < this->m_objects_pixel_data[local_pixel_group_index].pixels_data.size() - 1; i++) {
            if(this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i].second > this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1].second) {
                local_sorting_buffer.second = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i];
                local_sorting_buffer.first = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1];

                this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i] = local_sorting_buffer.first;
                this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1] = local_sorting_buffer.second;
            }

            else if(this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i].second == this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1].second
            && this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i].first > this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1].first) {
                local_sorting_buffer.second = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i];
                local_sorting_buffer.first = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1];

                this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i] = local_sorting_buffer.first;
                this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1] = local_sorting_buffer.second;
            }
        }

        // get boundaries 
        dengUIPixelBoundariesMode local_dengui_pixel_boundaries_mode;

        for(size_t i = 0; i < this->m_objects_pixel_data[local_pixel_group_index].pixels_data.size() - 1; i++) {
            if(this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i].second == vertical_index) {
                if(this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i - 1].second != vertical_index && this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1].second != vertical_index) {
                    boundaries.first = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i];
                    boundaries.second = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i];
                }
                
                else if(this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i - 1].second == vertical_index) {
                    boundaries.second = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i];
                }

                else if(this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i + 1].second == vertical_index) {
                    boundaries.first = this->m_objects_pixel_data[local_pixel_group_index].pixels_data[i];
                }
            }
        }
    }

    // sorting priority is vertical and then horisontal
    void PixelCollision::createPixelDataBoundaries() {
        for(PixelGroup &pixel_group : this->m_objects_pixel_data) {
            pixel_group.horisontal_pixel_boundaries.resize(this->m_p_window->getSize().second);

            for(size_t i = 0; i < pixel_group.horisontal_pixel_boundaries.size(); i++)
                getRowPixelBoundaries(i, pixel_group.horisontal_pixel_boundaries[i], pixel_group.object_description);
            
            pixel_group.pixels_data.clear();
        }
    }

    void PixelCollision::setupPixelRangesFromVertices(deng::vec2<size_t> *vertices_bounds, deng::vec2<size_t> *indices_bounds, const char *p_object_description) {
        this->m_objects_pixel_data.resize(this->m_objects_pixel_data.size() + 1);
        size_t local_obj_pix_size = this->m_objects_pixel_data.size() - 1;

        LOG("m_objects_pixel_data size: " + std::to_string(this->m_objects_pixel_data.size()));
        this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].object_description = p_object_description;
        
        std::vector<deng::UIVerticesData> local_object_vertices;
        local_object_vertices.resize(vertices_bounds->second - vertices_bounds->first);

        LOG("vertices bounds: " + std::to_string(vertices_bounds->first) + "-" + std::to_string(vertices_bounds->second));
        LOG("indices bounds: " + std::to_string(indices_bounds->first) + "-" + std::to_string(indices_bounds->second));

        std::vector<uint16_t> local_object_indices;
        local_object_indices.resize(indices_bounds->second - indices_bounds->first);
        LOG("local_object_indices size: " + std::to_string(local_object_indices.size()));

        for(size_t i = vertices_bounds->first, local_i = 0; i < vertices_bounds->second; i++, local_i++)
            local_object_vertices[local_i] = (*this->m_p_vertices_data)[i];
        
        for(size_t i = indices_bounds->first, local_i = 0; i < indices_bounds->second; i++, local_i++)
            local_object_indices[local_i] = (*this->m_p_indices_data)[i];
        
        uint16_t local_smallest_index = deng::HandleData::getSmallestElement<uint16_t>(&local_object_indices);
        for(size_t i = 0; i < local_object_indices.size(); i++) {
            local_object_indices[i] -= local_smallest_index;
            LOG("new local object index: " + std::to_string(local_object_indices[i]));
        }

        deng::vec3<deng::vec2<float>> local_triangle_vertices;
        deng::vec3<deng::vec2<float>> local_triangle_vectors;

        for(size_t i = 0; i <= local_object_indices.size() - 3; i += 3) {
            local_triangle_vertices.first =  {local_object_vertices[local_object_indices[i]].position_vec.first, local_object_vertices[local_object_indices[i]].position_vec.second};
            local_triangle_vertices.second = {local_object_vertices[local_object_indices[i + 1]].position_vec.first, local_object_vertices[local_object_indices[i + 1]].position_vec.second};
            local_triangle_vertices.third =  {local_object_vertices[local_object_indices[i + 2]].position_vec.first, local_object_vertices[local_object_indices[i + 2]].position_vec.second};

            local_triangle_vectors.first = local_triangle_vertices.second - local_triangle_vertices.first;
            local_triangle_vectors.second = local_triangle_vertices.third - local_triangle_vertices.second;
            local_triangle_vectors.third = local_triangle_vertices.first - local_triangle_vertices.third;

            for(size_t i = 0; i < local_triangle_vectors.size(); i++) {
                LOG("triangle vectors are: " + std::to_string((*local_triangle_vectors.getVectorElement(i)).first) + "/" + std::to_string((*local_triangle_vectors.getVectorElement(i)).second));
            }

            for(double i = static_cast<double>(local_triangle_vertices.first.first); round((i + 1.0) / this->m_p_window->getPixelSize().first) < round(static_cast<double>(local_triangle_vertices.second.first + 1) / this->m_p_window->getPixelSize().first); i += this->m_p_window->getPixelSize().first) {
                deng::vec2<double> local_new_pixel_coords;
                local_new_pixel_coords.first = round((i + 1.0) / this->m_p_window->getPixelSize().first);

                if(local_triangle_vectors.first.second < 0)
                    local_new_pixel_coords.second = round((-(local_triangle_vectors.first.second / local_triangle_vectors.first.first) * (i - static_cast<double>(local_triangle_vertices.first.first)) + static_cast<double>(local_triangle_vertices.first.second + 1.0f)) / this->m_p_window->getPixelSize().second);
                
                else 
                    local_new_pixel_coords.second = round(((local_triangle_vectors.first.second / local_triangle_vectors.first.first) * (i - static_cast<double>(local_triangle_vertices.first.first)) + static_cast<double>(local_triangle_vertices.first.second + 1.0f)) / this->m_p_window->getPixelSize().second);
                this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].pixels_data.push_back(local_new_pixel_coords);
            }

            if(local_triangle_vertices.second.first <= local_triangle_vertices.third.first) {
                for(double i = static_cast<double>(local_triangle_vertices.second.first); round((i + 1.0) / this->m_p_window->getPixelSize().first) < round(static_cast<double>(local_triangle_vertices.third.first + 1) / this->m_p_window->getPixelSize().first); i += this->m_p_window->getPixelSize().first) {
                    deng::vec2<double> local_new_pixel_coords;
                    local_new_pixel_coords.first = round((i + 1.0) / this->m_p_window->getPixelSize().first);

                    if(local_triangle_vectors.second.second < 0)
                        local_new_pixel_coords.second = round((-(local_triangle_vectors.second.second / local_triangle_vectors.second.first) * (i - static_cast<double>(local_triangle_vertices.second.first)) + static_cast<double>(local_triangle_vertices.second.second + 1.0f)) / this->m_p_window->getPixelSize().second);
                    
                    else 
                        local_new_pixel_coords.second = round(((local_triangle_vectors.second.second / local_triangle_vectors.second.first) * (i - static_cast<double>(local_triangle_vertices.second.first)) + static_cast<double>(local_triangle_vertices.second.second + 1.0f)) / this->m_p_window->getPixelSize().second);

                    this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].pixels_data.push_back(local_new_pixel_coords);
                }  
            }

            else if(local_triangle_vertices.second.first > local_triangle_vertices.third.first) {
                for(double i = static_cast<double>(local_triangle_vertices.second.first); round((i + 1.0) / this->m_p_window->getPixelSize().first) > round(static_cast<double>(local_triangle_vertices.third.first + 1) / this->m_p_window->getPixelSize().first); i -= this->m_p_window->getPixelSize().first) {
                    deng::vec2<double> local_new_pixel_coords;
                    local_new_pixel_coords.first = round((i + 1.0) / this->m_p_window->getPixelSize().first);

                    if(local_triangle_vectors.second.second < 0)
                        local_new_pixel_coords.second = round((-(local_triangle_vectors.second.second / local_triangle_vectors.second.first) * (i - static_cast<double>(local_triangle_vertices.second.first)) + static_cast<double>(local_triangle_vertices.second.second + 1.0f)) / this->m_p_window->getPixelSize().second);
                    
                    else 
                        local_new_pixel_coords.second = round(((local_triangle_vectors.second.second / local_triangle_vectors.second.first) * (i - static_cast<double>(local_triangle_vertices.second.first)) + static_cast<double>(local_triangle_vertices.second.second + 1.0f)) / this->m_p_window->getPixelSize().second);

                    this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].pixels_data.push_back(local_new_pixel_coords);
                }  
            }

            for(double i = static_cast<double>(local_triangle_vertices.third.first); round((i + 1.0) / this->m_p_window->getPixelSize().first) > round(static_cast<double>(local_triangle_vertices.first.first + 1) / this->m_p_window->getPixelSize().first); i -= this->m_p_window->getPixelSize().first) {
                deng::vec2<double> local_new_pixel_coords;
                local_new_pixel_coords.first = round((i + 1.0) / this->m_p_window->getPixelSize().first);

                if(local_triangle_vectors.third.second < 0)
                    local_new_pixel_coords.second = round(((local_triangle_vectors.third.second / local_triangle_vectors.third.first) * (i - static_cast<double>(local_triangle_vertices.third.first)) + static_cast<double>(local_triangle_vertices.third.second + 1.0f)) / this->m_p_window->getPixelSize().second);
                
                else
                    local_new_pixel_coords.second = round((-(local_triangle_vectors.third.second / local_triangle_vectors.third.first) * (i - static_cast<double>(local_triangle_vertices.third.first)) + static_cast<double>(local_triangle_vertices.third.second + 1.0f)) / this->m_p_window->getPixelSize().second);
                
                this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].pixels_data.push_back(local_new_pixel_coords);
            }

            for(deng::vec2<double> &pixel_data : this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].pixels_data) {
                LOG("Pixel coords: " + std::to_string(pixel_data.first) + "/" + std::to_string(pixel_data.second));
            }

            LOG("Successfully rasterised triangle vertices!");
                // this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].pixels_data.resize(this->m_objects_pixel_data[this->m_objects_pixel_data.size() - 1].pixels_data.size() + 3);
        
            this->createPixelDataBoundaries();
        }
    }

    dengBool PixelCollision::isCollided(const char *p_object_description, const deng::vec2<size_t> &colliding_pixel) {
        if(colliding_pixel.first >= 0 && colliding_pixel.second >= 0) {
            size_t local_pixel_group_index = 0;
            while (this->m_objects_pixel_data[local_pixel_group_index].object_description != p_object_description && local_pixel_group_index < this->m_objects_pixel_data.size()) local_pixel_group_index++;
            
            if(colliding_pixel.first >= this->m_objects_pixel_data[local_pixel_group_index].horisontal_pixel_boundaries[static_cast<size_t>(colliding_pixel.second)].first.first &&
            colliding_pixel.first <= this->m_objects_pixel_data[local_pixel_group_index].horisontal_pixel_boundaries[static_cast<size_t>(colliding_pixel.second)].second.first) {
                return DENG_TRUE;
            }
        }

        return DENG_FALSE;
    }
}