#ifndef ASSET_PACKAGE_HPP
#define ASSET_PACKAGE_HPP

#include "sosim/SDL_Factory.hpp"


namespace asset
{
    struct AssetPackage
    {
    public:
        static AssetPackage* get_instance();

        sosim::u_ptr<TTF_Font> m_d2coding;

        
    private:
        AssetPackage() {}
        static AssetPackage* s_instance;
    };
}


#endif