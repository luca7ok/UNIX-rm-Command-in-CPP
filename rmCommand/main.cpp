#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstring>

namespace fs = std::filesystem;
using namespace std;

void print_help() {
    cout << "Usage: rm [OPTION]... [FILE]...\n";
    cout << "Remove(unlink) the FILE(s).\n\n";
    cout << "-f, --force           ignore nonexistent files and arguments, never prompt\n";
    cout << "-r, -R, --recursive   remove directories and their contents recursively\n";
    cout << "-d, --dir             remove empty directories\n";
    cout << "-v, --verbose         explain what is being done\n";
}

void removeRecursive(const filesystem::path& path) {
    for (auto& file : filesystem::directory_iterator(path)) {
        if (filesystem::is_directory(file)) {
            removeRecursive(file);
        }
        else {
            cout << "removed '" << file.path().string() << "'\n";
        }
    }
    cout << "removed directory '" << path.string() << "'\n";
}

int main(int argc, char* argv[]) {
    bool recursive = false;
    bool force = false;
    bool empty = false;
    bool verbose = false;
    vector<string> files;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-f" || arg == "--force") {
            force = true;
        }
        else if (arg == "-r" || arg == "-R" || arg == "--recursive") {
            recursive = true;
        }
        else if (arg == "-d" || arg == "--dir") {
            empty = true;
        }
        else if (arg == "-v" || arg == "-verbose") {
            verbose = true;
        }
        else if (arg == "--help") {
            print_help();
            return 0;
        }
        else if (arg[0] == '-') {
            cout << "rm: invalid option -- '" << arg[1] << "'\n";
            cout << "Try 'rm --help' for more information.\n";
            return 1;
        }
        else {
            files.push_back(arg);
        }
    }

    if (files.empty()) {
        cout<<"rm: missing operand\n";
        cout << "Try 'rm --help' for more information.\n";
        return 1;
    }

    for (auto& file : files) {
        if (!filesystem::exists(file)) {
            if (!force) {
                cout << "rm: cannot remove '" << file << "': No such file or directory\n";
            }
        }

        else if (filesystem::is_directory(file) && !recursive && !empty) {
            cout << "rm: cannot remove '"<<file<<"': Is a directory\n";
        }

        else if (filesystem::is_directory(file)) {
            if (filesystem::is_empty(file) && (empty || recursive)) {
                if (verbose) {
                    cout << "removed directory '" << file << "'\n";
                }
                filesystem::remove(file);
            }

            else if (!filesystem::is_empty(file) && empty && !recursive) {
                cout << "rm: cannot remove '" << file << "': Directory not empty\n";
            }

            else if (!filesystem::is_empty(file) && recursive) {
                if (verbose) {
                    removeRecursive(file);
                }
                filesystem::remove_all(file);
            }
        }

        else {
            if (verbose) {
                cout << "removed '" << file << "'\n";
            }
            filesystem::remove(file);
        }
    }


    return 0;
}