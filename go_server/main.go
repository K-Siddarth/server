package main

import (
	"fmt"
	"go_server/utils"
)

func format_problem_name(problem_name string) string {
	final_problem_name := ""
	for _, c := range problem_name {
		if c == '.' {
			continue
		}
		if c == ' ' {
			c = '_'
		}
		final_problem_name += string(c)
	}
	return string(final_problem_name)
}

func main() {
	const port = 27121
	fmt.Println("Main function running...")
	parsed_data := utils.Server(port)
	problem_name := format_problem_name(parsed_data.Name)
	var input_data []string
	var output_data []string
	for _, test := range parsed_data.Tests {
		input_data = append(input_data, test.Input)
		output_data = append(output_data, test.Output)
	}
	utils.Create_files(problem_name, input_data, output_data)
}
