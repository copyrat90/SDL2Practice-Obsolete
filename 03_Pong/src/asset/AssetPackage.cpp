#include "asset/AssetPackage.hpp"


namespace asset
{
    AssetPackage* AssetPackage::s_instance = nullptr;

    AssetPackage* AssetPackage::get_instance()
    {
        if (!s_instance)
        {
            s_instance = new AssetPackage();
        }
        return s_instance;
    }
}
