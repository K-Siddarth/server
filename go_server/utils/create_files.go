package utils

import (
	"fmt"
	"os"
	"path/filepath"
)

func Create_files(problem_name string, input_data []string, output_data []string) {
	dir, err := os.Getwd() // gives the dir in which the executable was run
	if err != nil {
		panic(err)
	}
	if len(input_data) != len(output_data) {
		panic("length of input_data and output_data are not same in the data given to create_file")
	}
	fmt.Println("Creating a new dir in ", dir, " with the name", problem_name, "with permissionrwxrwxr-x")
	err = os.Mkdir(problem_name, 0775)
	if err != nil {
		fmt.Println("Error in creating file")
		panic(err)
	}
	contest_dir := filepath.Join(dir, problem_name)
	for i := 0; i < len(input_data); i++ {
		input_file_name := fmt.Sprintf("input%d.in", i+1)
		input_file_path := filepath.Join(contest_dir, input_file_name)
		err = os.WriteFile(input_file_path, []byte(input_data[i]), 0660)
		if err != nil {
			fmt.Println("Error in input file creating")
			panic(err)
		}
		output_file_name := fmt.Sprintf("output%d.out", i+1)
		output_file_path := filepath.Join(contest_dir, output_file_name)
		err = os.WriteFile(output_file_path, []byte(output_data[i]), 0660)
		if err != nil {
			fmt.Println("Error in output file creating")
			panic(err)
		}
	}
}
