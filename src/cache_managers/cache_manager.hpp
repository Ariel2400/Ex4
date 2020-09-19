#include <string>

class CacheManager {
    public:
        virtual bool is_file_in_cache(const std::string action_description) = 0;
        virtual void store_into(const std::string src_file_path, const std::string action_description) = 0;
        virtual void load_from(const std::string action_description, const std::string dest_file_path) = 0;
};