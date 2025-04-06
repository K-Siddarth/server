#include <sys/socket.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
// #include <sys/stat.h>
#include <fstream>
#include <filesystem>

#define PORT 27121
#define BUFFER_SIZE 30720
#define QUEUE_SIZE 3

std::string CP_DIR="/home/siddarth/cp/";
std::string TEMPLATE_DIR="/home/siddarth/cp/template/";

using json = nlohmann::json;

int main() {
  sockaddr_in server_address;
  int new_socket, server_fd;
  socklen_t addrlen=sizeof(server_address);
  char buffer[BUFFER_SIZE];

  // making a socket
  server_fd=socket(AF_INET, SOCK_STREAM, 0);
  if(server_fd==0) {
    perror("Socket creation failed");
    return 1;
  }

  // binding the socket to server;
  server_address.sin_family=AF_INET;
  server_address.sin_addr.s_addr=INADDR_ANY; // TODO: check what is this
  server_address.sin_port=htons(PORT);

  if(bind(server_fd, (sockaddr*) &server_address, addrlen)<0) {
    perror("Binding failed");
    return 1;
  }

  // listening 
  if (listen(server_fd,  QUEUE_SIZE)<0) {
    perror("Listening failed");
    return 1;
  }

  std::cout << "Listening on PORT: "  << PORT << "...\n";

  while(true) {
    new_socket=accept(server_fd, (sockaddr*) &server_address, &addrlen);
    if(new_socket<0) {
      perror("Accept failed: Did not create new socket");
      return 1;
    }

    int response=read(new_socket, buffer, BUFFER_SIZE);
    if(response<=0) {
      close(new_socket);
      continue;
    }

    buffer[response]='\0';
    std::string request(buffer);

    std::istringstream stream(request);
    std::string line;

    int content_length;

    // reading content length
    while(std::getline(stream, line)&&line!="\r") {
      int cont_len_ind=line.find("Content-Length:");
      if(cont_len_ind!=std::string::npos) {
        content_length=std::stoi(line.substr(cont_len_ind+15));
        break;
      }
    }
    std::cout << "Parsed Content-Length: " << content_length << "\n";

    // reading body
    std::string body="temp body";
    while(body!="\r") {
      std::getline(stream, body);
    }
    std::getline(stream, body, '\0');

    std::cout << "========================Request's Body: ========================\n" << body << "\n";

    std::cout << "========================JSON DATA: ========================\n";
    json j=json::parse(body);
    std::cout << j << "\n";

// ========================JSON DATA: ========================
// {"batch":{"id":"6c2dca08-b637-438a-9899-15a67582dc2b","size":1},"group":"Codeforces - Codeforces Round 1011 (Div. 2)","input":{"type":"stdin"},"interactive":false,"languages":{"java":{"mainClass":"Main","taskClass":"BServalAndFinalMEX"}},"memoryLimit":256,"name":"B. Serval and Final MEX","output":{"type":"stdout"},"testType":"single","tests":[{"input":"6\n4\n1 2 3 4\n5\n0 1 0 0 1\n6\n0 0 0 0 0 0\n6\n5 4 3 2 1 0\n4\n0 0 1 1\n4\n1 0 0 0\n","output":"1\n1 4\n4\n1 2\n1 2\n1 2\n1 2\n4\n5 6\n3 4\n1 2\n1 3\n3\n4 5\n4 5\n1 4\n2\n1 2\n1 3\n2\n2 4\n1 2\n"}],"timeLimit":1000,"url":"https://codeforces.com/contest/2085/problem/B"}

    std::string contest_name="unknown_contest";
    std::string group=j["group"];

    // reading contest_name:
    int cont_name_ind=group.find(" - ");
    if(cont_name_ind!=std::string::npos) {
      contest_name=group.substr(cont_name_ind+3);
    }
    std::cout << "Read Contest name: " << contest_name << "\n";

    std::string problem_name="unknown_problem";
    if(j.contains("name")) {
      problem_name=j["name"];
    }

    // making dirs and copying contents from template dir:
    std::string contest_dir=CP_DIR+contest_name+"/";

    // file permission bits: 
    // 7 means read, write and execute. 
    // 6 means read and write. 
    // 5 means read and execute. 
    // 4 means read only. 

    // creating problem dir:
    // int status=mkdir(contest_dir.c_str(), 0777);

    std::string problem_dir_path=contest_dir+problem_name+"/";

    std::filesystem::path problem_dir=problem_dir_path;
    std::filesystem::create_directories(problem_dir);
    std::cout << "Created " << problem_name << " dir at " << problem_dir_path << "\n";

    int i=0;
    // writing sample.in, sample.out files:
    for(auto& test: j["tests"]) {
      ++i;
      std::string input=test["input"];
      std::string output=test["output"];

      std::string in_file_path=problem_dir_path+"sample"+std::to_string(i)+".in";
      std::string out_file_path=problem_dir_path+"sample"+std::to_string(i)+".out";

      std::ofstream inp_file_stream(in_file_path);
      std::ofstream out_file_stream(out_file_path);

      if(inp_file_stream.is_open()) {
        inp_file_stream << input;
        inp_file_stream.close();
      }
      else {
        std::cerr << "Failed to open input file: sample" << i << ".in\n";
      }
      if(out_file_stream.is_open()) {
        out_file_stream << output;
        out_file_stream.close();
      }
      else {
        std::cerr << "Failed to open output file: sample" << i << ".out\n";
      }
      std::cout << "Made Sample" << i << " files at" << problem_dir_path << "\n";
    }
    // copying from template dir:
    std::filesystem::path in_sol_dir=TEMPLATE_DIR+"sol.cpp";
    std::filesystem::path in_makeFile_dir=TEMPLATE_DIR+"Makefile";

    std::filesystem::path out_sol_dir=problem_dir_path+"sol.cpp";
    std::filesystem::path out_makeFile_dir=problem_dir_path+"Makefile";

    try {
      std::filesystem::copy(in_sol_dir, out_sol_dir);
      std::filesystem::copy(in_makeFile_dir, out_makeFile_dir);
    }
    catch(std::filesystem::filesystem_error& e) {
      std::cerr << "Filesystem error: " << e.what() << "\n";
    }

    std::cout << "Copied files from " << TEMPLATE_DIR << " to " << problem_dir_path << "\n\n";
  }


  return 0;
}
