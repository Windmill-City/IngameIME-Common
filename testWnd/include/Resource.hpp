#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

struct ResourceLocation
{
    const static char DOMAIN_SEPARATOR;

    /**
     * @brief Resource n
     * eg: default:shader/VertexShader.txt, the domain is 'default'
     */
    const std::string Domain;
    /**
     * @brief Resource path
     * eg: default:shader/VertexShader.txt, the path is 'shader/VertexShader.txt'
     */
    const std::string Path;

    /**
     * @brief Construct a new Resource Location object
     *
     * @param key resource key, eg: default:shader/VertexShader.txt
     */
    ResourceLocation(const std::string& key);

    /**
     * @brief Construct a new Resource Location object
     *
     * @param domain resource domain
     * @param path resource path
     */
    ResourceLocation(const std::string& domain, const std::string& path) noexcept;

    bool operator==(const ResourceLocation& b) const noexcept;
    bool operator<(const ResourceLocation& b) const noexcept;

    /**
     * @brief Get domain of the resource key
     *
     * @return std::string_view domain
     */
    static std::string getDomain(const std::string& key);

    /**
     * @brief Get the path of the resource key
     *
     * @return std::string_view path
     */
    static std::string getPath(const std::string& key);
};

using ResourceStream = std::unique_ptr<std::istream>;

struct ResourceProvider
{
    /**
     * @brief Get file stream by ResourceLocation
     *
     * @param loc resource location
     * @return ResourceStream a valid stream or nullptr
     */
    virtual ResourceStream get(const ResourceLocation& loc) = 0;

    ResourceProvider() = default;

    // Global object, should not copy or move
    ResourceProvider(const ResourceProvider&)             = delete;
    ResourceProvider& operator=(const ResourceProvider&)  = delete;
    ResourceProvider(const ResourceProvider&&)            = delete;
    ResourceProvider& operator=(const ResourceProvider&&) = delete;
};

struct ResourceManager : public ResourceProvider
{
    using ProviderHolder = std::unordered_map<std::string, std::unique_ptr<ResourceProvider>>;

    const static std::string DEFAULT_ASSETS_DIR;
    const static std::string DEFAULT_DOMAIN;
    const static std::string EMBED_DOMAIN;

    /**
     * @brief Domain providers
     * Key: Domain, Value: Provider
     */
    ProviderHolder Providers;

    ResourceManager();
    virtual ResourceStream get(const ResourceLocation& loc) override;
};

struct EmbedResource : public ResourceProvider
{
    struct membuf : std::streambuf
    {
        membuf(const uint8_t* base, size_t size);
        pos_type seekoff(off_type               _Off,
                         std::ios_base::seekdir _Way,
                         std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;
        pos_type seekpos(pos_type, std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override;
    };

    struct imstream
        : virtual membuf
        , std::istream
    {
        imstream(const uint8_t* base, size_t size);
    };

    /**
     * @brief Resource index
     * Key: Path, Val:Pair<Offset,Size>
     */
    using IndexHolder = std::unordered_map<std::string, const std::pair<size_t, size_t>>;

    /**
     * @brief Resource domain
     *
     */
    const std::string  Domain;
    /**
     * @brief Resource block, contains continuously stored files
     *
     */
    const uint8_t*     Block;
    const IndexHolder  Index;

    EmbedResource(const IndexHolder index, const uint8_t* block);
    EmbedResource(const uint8_t* index, const uint8_t* block);
    virtual ResourceStream get(const ResourceLocation& loc) override;

    static IndexHolder _make_index(const uint8_t* index);

    template <class T, typename = std::enable_if_t<std::is_trivial_v<T>>>
    static T _get(const uint8_t* index, size_t& offset)
    {
        auto val = *(T*)&index[offset];
        offset += sizeof(T);
        return val;
    }

    static std::string _get_path(const uint8_t* index, size_t& offset);
};

struct FileResource : public ResourceProvider
{
    const std::filesystem::path Root;

    FileResource(const std::string& root);
    virtual ResourceStream get(const ResourceLocation& loc) override;
};