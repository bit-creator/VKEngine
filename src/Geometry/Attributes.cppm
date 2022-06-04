export module Geometry.Attributes;

import Vulkan;
import <optional>;
import <array>;
import <vector>;
import <map>;
import <iostream>;

export enum class Attribute {
    Position  =0,
    Normal    =1,
    Tangent   =2,
    BiTangent =3,
    Texture   =4,
    Color     =5
};

constexpr static int NUM_ATTRIBUTES = 6;
using WholeAttribDescr = std::map<Attribute, VkVertexInputAttributeDescription>;

export class Attributes {
private:
    WholeAttribDescr                 _attributes;
    size_t                           _hash;

public:
    Attributes();
    void enable(Attribute attr);
    void disable(Attribute attr);
    void enableAll();
    void disableAll();
    bool hasAttribute(Attribute attr) const;
    std::size_t getAttribHash() const;

    void add(Attribute attr, uint32_t offset, VkFormat format);

    std::vector<VkVertexInputAttributeDescription> getDescriptions();
};

Attributes::Attributes()
    : _attributes()
    , _hash(0) {
}

void Attributes::enable(Attribute attr) {
    // std::cout << "enable" << (int)attr << std::endl;
    _hash |= 1 << (size_t)attr;
}

void Attributes::disable(Attribute attr) {
    _hash &= ~(1 << (size_t)attr);    
}

void Attributes::enableAll() {
    for(auto& [attr, descr]: _attributes) enable(attr);
}

void Attributes::disableAll() {
    for(auto& [attr, descr]: _attributes) disable(attr);
}

void Attributes::add(Attribute attr, uint32_t offset, VkFormat format) {
    _attributes[attr] = VkVertexInputAttributeDescription {
        (uint32_t)attr, 0, format, offset
    };
    enable(attr);
}

bool Attributes::hasAttribute(Attribute attr) const {
    return (_hash >> (uint32_t)attr) & 1;
}

std::size_t Attributes::getAttribHash() const {
    return _hash;
}
std::vector<VkVertexInputAttributeDescription> Attributes::getDescriptions() {
    std::vector<VkVertexInputAttributeDescription> ret;
        // std::cout << _hash << std::endl;
    for(auto& [attr, descr]: _attributes) {

        
        // std::cout << "getDescr" << std::endl;
        if(hasAttribute(attr)) ret.push_back(descr);

    }
    return ret;
}