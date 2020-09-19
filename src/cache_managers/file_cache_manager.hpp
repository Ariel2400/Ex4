#include <string>

class Cache {
public:
  /* returns true if a file that was created by the described action is in
    cache, else false */
  bool is_file_in_cache(const std::string action_description);

  /* searches the cache for a file that was created by the described action, and
     prints a message */
  void search(const std::string action_description);

  /* prints the file that was created by the described action from cache.
     the method assumes that the file exists in cache */
  void print(const std::string action_description);

  // adds file that was created by the described action to cache
  void store_into(const std::string src_file_path,
                         const std::string action_description);

  // retrieves a file that was created by the described action from cache
  void load_from(const std::string action_description,
                        const std::string dest_file_path);

  // clears the cache
  void clear();

private:
  // creates the file "file_list.txt" in cache if it doesn't already exist
  void create_file_list();

  // returns the result file's suffix based on the action described
  std::string get_suffix(const std::string action_description);

  /* returns the described action's file's serial number in cash.
     returns 0 if the file is not in cache.
     returns -1 if an error occurred */
  int get_action_serial_number(const std::string action_description);

  /* returns the serial number of the next file to be added
  returns -1 if an error occurred */
  int get_next_serial_number();
};