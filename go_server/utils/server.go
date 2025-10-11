package utils

import (
	"fmt"
	"net/http"
	"encoding/json"
)

/*
{"name":"A. Candies for Nephews","group":"Codeforces - Educational Codeforces Round 183 (Rated for Div. 2)","url":"https://codeforces.com/contest/2145/problem/A","interactive":false,"memoryLimit":256,"timeLimit":1000,"tests":[{"input":"2\n7\n24\n","output":"2\n0\n"}],"testType":"single","input":{"type":"stdin"},"output":{"type":"stdout"},"languages":{"java":{"mainClass":"Main","taskClass":"ACandiesForNephews"}},"batch":{"id":"49913b3a-f2b4-4c10-87fe-dc03386f8b63","size":1}}
*/

type ProblemData struct {
	Name  string `json:"name"`
	Tests []struct {
		Input  string `json:"input"`
		Output string `json:"output"`
	} `json:"tests"`
}

var json_data ProblemData

func Server(port int) ProblemData {
	port_string := fmt.Sprintf(":%d", port)

	server := &http.Server{
		Addr: "localhost"+port_string,
	}

	http.HandleFunc("/", func (w http.ResponseWriter, r *http.Request) {
		var req ProblemData
		err := json.NewDecoder(r.Body).Decode(&req)
		if err != nil {
			panic(err)
		}
		json_data = req
		server.Close()
	}) // since we will get requests on the root only

	err := server.ListenAndServe()
	if err != nil {
		fmt.Println("Error: ", err)
	} else {
		fmt.Println("Server running on ", port)
	}
	return json_data 
}
