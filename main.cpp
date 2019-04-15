#include <iostream>
#include <vector>
#include <dirent.h>
#include <cstring>
#include "filter.h"
#include <queue>
#include <limits>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

std::queue<char*> queue;
char* converse_to_char(std::string const &s){
    char * buff = new char[s.size()+1];
    return strcpy(buff,s.data());

}
bool matchSize(filter const &userRequest,struct stat &info) {
    if (userRequest.needSize) {
        if (userRequest.neededSize == userRequest.EQUAL)
            return userRequest.size == info.st_size;
        if(userRequest.neededSize== userRequest.GREATER){
            return userRequest.size < info.st_size;
        }
        if(userRequest.neededSize == userRequest.LESS){
            return userRequest.size > info.st_size;
        }
    }
    return true;
}
bool matchNode(filter const &userRequest,struct stat &info) {
    if (userRequest.needNodeNumber) {
        return userRequest.iNodeNumber == info.st_ino;
    }
    return true;
}
bool matchName(filter const &userRequest,std::string name) {
    if (userRequest.needName) {
        return userRequest.name == name;
    }
    return true;
}
bool matchHardLinks(filter const &userRequest,struct stat &info) {
    if (userRequest.needNumberOfHardLinks) {
        return userRequest.numberOfHardlinks == info.st_nlink;
    }
    return true;
}
std::vector<char*> bfs(filter const &userRequest,std::string const &absolutePath){
    std::vector<char*> output;
    DIR* directory = opendir(absolutePath.c_str());
    queue.push(converse_to_char(absolutePath));
    while (!queue.empty()){
        char * path  = queue.front();
        queue.pop();
        directory = opendir(path);
        if(!directory){
            continue;
        }
        while(dirent* fileInside = readdir(directory)){
            std::string name = fileInside->d_name;
            if(name=="."||name==".."){
                continue;
            }
            struct stat info;
            std::string relativePath = path;
            relativePath.append("/").append(name);

            if(stat(relativePath.c_str(),&info)==-1){
                std::cerr<<"bad path of "<<name<<"\n";
                continue;
            }
            if(S_ISDIR(info.st_mode)){
                queue.push(converse_to_char(relativePath));
            }
            if(S_ISREG(info.st_mode)){
                if(matchHardLinks(userRequest,info)&&matchName(userRequest,name)
                &&matchNode(userRequest,info)&&matchSize(userRequest,info)){
                    output.push_back(converse_to_char(name));
                }
            }
        }
    }
    closedir(directory);
    return output;
}
void execute(std::string const & path,std::vector<char*> args){
    pid_t pid = fork();
    args.push_back(nullptr);
    if (pid < 0) {            //:(
        std::cout << "Can't create  process" << '\n';
    } else if (pid == 0) {//child
        if (execve(path.c_str(), args.data(), nullptr) == -1) {
            perror("Execution failed");
            exit(-1);
        }
        exit(0);
    } else {         //parent
        int res;
        if (waitpid(pid, &res, 0) == -1) {
            perror("Can't finish process");
        } else {
            std::cout << "Return: " << res << '\n';
        }
    }
}

void finish(filter & userRequest,std::string & absolutePath){
    std::vector<char*> output = bfs(userRequest,absolutePath);
    if(userRequest.needExec){
        execute(userRequest.exec,output);
    }
    else {
        for(auto i : output){
            std::cout<<i<<"\n";
        }
    }
}
void prepare(int argc,char*argv[]){
    std::vector<char*> arguments;
    for (size_t i = 1; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }
    std::string absolutePath = arguments[0];
    filter userRequest;
    for (size_t i = 1; i < arguments.size(); i += 2) {
        userRequest.add(arguments[i], arguments[i + 1]);
    }
    finish(userRequest,absolutePath);
}
int main(int argc, char *argv[]) {
    if (argc < 2||argc%2==1) {
        std::cerr << "Wrong number of arguments";
        exit(EXIT_FAILURE);
    }
    prepare(argc,argv);

}