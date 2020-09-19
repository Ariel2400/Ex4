#include <dirent.h>
#include <fstream>
#include <iostream>

#include "file_cache_manager.hpp"

#define FILE_LIST_PATH "./cache/file_list.txt"

void create_file_list() {
  std::ofstream create_file_list(FILE_LIST_PATH, std::ios::app);
  if (!create_file_list) {
    std::cerr << "Unable to create cache file list" << std::endl;
  } else {
    create_file_list.close();
  }
}

std::string get_suffix(const std::string action_description) {
  std::string department(
      action_description.substr(0, action_description.find(" ")));
  if (department.compare("matrix") == 0 || department.compare("hash") == 0) {
    return ".txt";
  }
  if (department.compare("image") == 0) {
    return ".bmp";
  }
  return "";
}

int get_action_serial_number(const std::string action_description) {
  int serial_number = 1;
  std::string line;
  create_file_list();
  std::ifstream file_path_input_stream(FILE_LIST_PATH);
  if (!file_path_input_stream) {
    std::cerr << "Unable to open cache file list1" << std::endl;
    return -1;
  }
  while (std::getline(file_path_input_stream, line)) {
    if (line.compare(action_description) == 0) {
      file_path_input_stream.close();
      return serial_number;
    }
    ++serial_number;
  }
  file_path_input_stream.close();
  return 0;
}

int get_next_serial_number() {
  int serial_number = 1;
  std::string line;
  create_file_list();
  std::ifstream file_path_input_stream(FILE_LIST_PATH);
  if (!file_path_input_stream) {
    std::cerr << "Unable to open cache file list2" << std::endl;
    return -1;
  }
  while (std::getline(file_path_input_stream, line)) {
    ++serial_number;
  }
  file_path_input_stream.close();
  return serial_number;
}

bool is_file_in_cache(const std::string action_description) {
  if (get_action_serial_number(action_description) == 0) {
    return false;
  }
  return true;
}

void search(const std::string action_description) {
  if (is_file_in_cache(action_description)) {
    std::cout << "result found in cache" << std::endl;
  } else {
    std::cout << "result not found in cache" << std::endl;
  }
}

void print(const std::string action_description) {
  int serial_number = get_action_serial_number(action_description);
  std::string suffix(get_suffix(action_description));
  std::ifstream file_input_stream("cache/" + std::to_string(serial_number) +
                                  suffix);
  if (!file_input_stream) {
    std::cerr << "Unable to open cache file number " << serial_number
              << std::endl;
  } else {
    std::cout << file_input_stream.rdbuf();
    file_input_stream.close();
  }
}

void store_into(const std::string src_file_path,
                       const std::string action_description) {
  if (!is_file_in_cache(action_description)) {
    std::ifstream src_file_input_stream("../" + src_file_path);
    if (!src_file_input_stream) {
      std::cerr << "Unable to open " << src_file_path << std::endl;
    } else {
      int next_serial_number = get_next_serial_number();
      std::string suffix(get_suffix(action_description));
      std::ofstream cache_file_output_stream(
          "cache/" + std::to_string(next_serial_number) + suffix);
      if (!cache_file_output_stream) {
        src_file_input_stream.close();
        std::cerr << "Unable to open cache file number " << next_serial_number
                  << std::endl;
      } else {
        cache_file_output_stream << src_file_input_stream.rdbuf();
        src_file_input_stream.close();
        cache_file_output_stream.close();
        std::ofstream file_list_output_stream(FILE_LIST_PATH, std::ios::app);
        if (!file_list_output_stream) {
          std::cerr << "Unable to open cache file list" << std::endl;
        } else {
          file_list_output_stream << action_description << std::endl;
          file_list_output_stream.close();
        }
      }
    }
  }
}

void load_from(const std::string action_description,
                      const std::string dest_file_path) {
  int serial_number = get_action_serial_number(action_description);
  std::string suffix(get_suffix(action_description));
  std::ifstream cache_file_input_stream("cache/" +
                                        std::to_string(serial_number) + suffix);
  if (!cache_file_input_stream) {
    std::cerr << "Unable to open cache file number " << serial_number
              << std::endl;
  } else {
    std::ofstream dest_file_output_stream("../" + dest_file_path);
    if (!dest_file_output_stream) {
      cache_file_input_stream.close();
      std::cerr << "Unable to open " << dest_file_path << std::endl;
    } else {
      dest_file_output_stream << cache_file_input_stream.rdbuf();
      cache_file_input_stream.close();
      dest_file_output_stream.close();
    }
  }
}

void clear() {
  DIR *cache = opendir("cache");
  struct dirent *next_file;
  std::string filepath;
  while ((next_file = readdir(cache)) != NULL) {
    std::string file_name(next_file->d_name);
    if (file_name.compare("file_list.txt") != 0) {
      std::string file_path("cache/" + file_name);
      remove(file_path.c_str());
    }
  }
  closedir(cache);
  std::ofstream file_list_output_stream;
  create_file_list();
  file_list_output_stream.open(FILE_LIST_PATH,
                               std::ofstream::out | std::ofstream::trunc);
  if (!file_list_output_stream) {
    std::cerr << "Unable to open cache file list3" << std::endl;
  } else {
    file_list_output_stream.close();
  }
}