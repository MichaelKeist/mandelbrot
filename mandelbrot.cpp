#include <iostream>
#include <vector>
#include </home/michael/fun_projects/cpp_learning/mandelbrot/bmp.cpp>
#include <Dense>
#include <math.h>
//#include <complex>
//#include <cmath>
//find if the function at a point escapes a set threshold after 100x iterations
int image_escapes(long double x, long double y, int threshold, int iterations){
	long double x0 = x;
	long double y0 = y;
	int count = 0;
	long double temp;
	//function goes to infinity if distance from center exceeds 2
	while(count < iterations and (x * x) + (y * y) < threshold){
		temp = (x * x) - (y * y) + x0;
		y = 2 * x * y + y0;
		x = temp;
		count++;
	}
	if(count == iterations){
		return 1;
	}else if(count > 500){
		return 2;
	}else if(count > 100){
		return 3;
	}else if(count > 50){
		return 4;
	}else if(count > 25){
		return 5;
	}
	return 0;
}
bool escapes(long double x, long double y, int threshold, int iterations){
	long double x0 = x;
	long double y0 = y;
	int count = 0;
	long double temp;
	//function goes to infinity if distance from center exceeds 2
	while(count < iterations and (x * x) + (y * y) < threshold){
		temp = (x * x) - (y * y) + x0;
		y = 2 * x * y + y0;
		x = temp;
		count++;
	}
	if(count == iterations){
		return false;
	}
	return true;
}
//maybe repurpose this function to check for x and y resolution
long double* collect_inputs(){
	long double* vals = (long double*)(calloc(3, sizeof(long double)));
	for(int n = 0; n < 3; n++){
		std::cin >> *(vals + n);
	}
	return vals;
}
int* generate_image(long double x, long double y, long double zoom, std::vector<std::vector<int>>& map){
	int resolutionx = 1500;
	int resolutiony = 1500;
	RGB_data buffer[1500][1500];
	memset(buffer, 0, sizeof(buffer));
	memset(buffer, 0, sizeof(buffer));
	long double xstep_size = (0.8/resolutionx)/zoom;
	long double ystep_size = (1.12/resolutiony)/zoom;
	long double xbegin = x - (xstep_size * (resolutionx/2));
	long double ybegin = y - (ystep_size * (resolutiony/2));
	for(int i = 0; i < resolutiony; i++){
		for(int j = 0; j < resolutionx; j++){
			int current = image_escapes(xbegin + xstep_size * j, ybegin + ystep_size * i, 4, 1000);
			if(current == 1){
				continue;
			}else if(current == 2){
				buffer[resolutionx-1-i][j].r = 0x40;
				buffer[resolutionx-1-i][j].g = 0x06;
				buffer[resolutionx-1-i][j].b = 0xbd;
			}else if(current == 3){
				buffer[resolutionx-1-i][j].r = 0x63;
				buffer[resolutionx-1-i][j].g = 0x51;
				buffer[resolutionx-1-i][j].b = 0xed;
			}else if(current == 4){
				buffer[resolutionx-1-i][j].r = 0x9e;
				buffer[resolutionx-1-i][j].g = 0x94;
				buffer[resolutionx-1-i][j].b = 0xeb;
			}else if(current == 5){
				buffer[resolutionx-1-i][j].r = 0xc7;
				buffer[resolutionx-1-i][j].g = 0xc2;
				buffer[resolutionx-1-i][j].b = 0xed;
			}else{
				buffer[resolutionx-1-i][j].r = 0xff;
				buffer[resolutionx-1-i][j].g = 0xff;
				buffer[resolutionx-1-i][j].b = 0xff;
			}
		}
		//std::cout << "\n";
	}
	bmp_generator("./test.bmp", 1500, 1500, (BYTE*)buffer);
	return 0;
}
int* find_steps(long double x, long double y, long double zoom, int resolutionx, int resolutiony, std::vector<std::vector<int>>& map){
	long double xstep_size = (0.8/resolutionx)/zoom;
	long double ystep_size = (1.12/resolutiony)/zoom;
	long double xbegin = x - (xstep_size * (resolutionx/2));
	long double ybegin = y - (ystep_size * (resolutiony/2));
	for(int i = 0; i < resolutiony; i++){
		for(int j = 0; j < resolutionx; j++){
			if(escapes(xbegin + xstep_size * j, ybegin + ystep_size * i, 4, 1000)){
				map[i][j] = 0;
				std::cout << ' ';
			}else{
				map[i][j] = 1;
				std::cout << "\u25A0";
			}
		}
		std::cout << "\n";
	}
	return 0;
}
float percent_filled(std::vector<std::vector<int>> map){
	//for(int i = 0; i < map.size(); i++){
	//	for(int j = 0; j < map.size(); j++){
	//		std::cout << map[i][j] << ' ';
	//	}
	//	std::cout << '\n';
	//}
	int dim = map.size();
	int count = 0;
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++){
			if(map[i][j] != 0){
				count++;
			}
		}
	}
	float answer = ((float)count / (dim * dim)) * 100;
	return answer;
}
//need to make a function for getting edges of the fractal
void edge_find(std::vector<std::vector<int>> map, std::vector<std::vector<int>>& edges){
	std::cout << "starting edge_find\n";
	std::vector<int> directions{-1,0,1};
	edges = map;
	bool is_edge = false;
	int dim = map.size();
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++){
			//if cell is part of set, check neighbors. if at least one neighbor
			//is not in set, the current cell is an edge
			if(map[i][j] != 0){
				is_edge = false;
				for(int horizontal: directions){
					for(int vertical: directions){
						if((i+vertical >= 0) and (i+vertical < dim) and (j+horizontal >= 0) and (j+horizontal < dim) and map[i+vertical][j+horizontal] == 0 and not (vertical == 0 and horizontal == 0)){
							is_edge = true;
							break;
						}
					}
					if(is_edge){
						edges[i][j] = 1;
						break;
					}
				}
				if(not is_edge){
					edges[i][j] = 0;
				}
			}
		}
	}
}
//box counting algorithm design:
//set box size equal to entire resolution
//while box size >= 1:
//count up number of boxes that contain an edge and save the value somewhere
//if box size is one, break
//halve box size and run again
//TO MAKE SURE BOXES CAN SPLIT EVENLY, EDGES BITMAP SHOULD BE A POWER OF 2 SIZE (1024 EG)
int box_check(std::vector<std::vector<int>> edges, int box_size, int i, int j){
	for(int sub_i = 0; sub_i < box_size; sub_i++){
		for(int sub_j = 0; sub_j < box_size; sub_j++){
			if(edges[i * box_size + sub_i][j * box_size + sub_j] == 1){
				return 1;
			}
		}
	}
	return 0;
} 
float linear_regression(std::vector<int> x, std::vector<int> y){	//return the regression slope of input coordinates
	Eigen::MatrixXd x_vals(x.size(), 2);
	Eigen::MatrixXd y_vals(y.size(), 1);
	for(int i = 0; i < x.size(); i++){
		x_vals(i,0) = x[i];
		x_vals(i,1) = 1;
	}
	for(int i = 0; i < y.size(); i++){
		y_vals(i, 0) = log2(y[i]);
	}
	Eigen::MatrixXd left = (x_vals.transpose()) * x_vals;
	Eigen::MatrixXd right = (x_vals.transpose()) * y_vals;
	Eigen::MatrixXd answer = left.ldlt().solve(right);
	return answer(0, 0);
}
float find_dimension(std::vector<std::vector<int>> edges){
	std::cout << "starting find_dimension\n";
	std::vector<int> log_size;
	std::vector<int> counts;
	int current_sum;
	int dim = edges.size();
	int box_row;
	int box_size = dim;
	while (box_size >= 8){ //minimum box size is 8 for speed
		std::cout << "box_size = " << box_size << "\n";
		current_sum = 0;
		box_row = dim / box_size;
		for(int i = 0; i < box_row; i++){
			for(int j = 0; j < box_row; j++){
				current_sum = current_sum + box_check(edges, box_size, i, j);
			}
		}
		log_size.push_back(counts.size());
		counts.push_back(current_sum);
		box_size = box_size / 2;
	}
	for(int i: counts){
		std::cout << i << '\n';
	}
	float slope = linear_regression(log_size, counts);
	std::cout << slope << '\n';
	return 0.0;
}
int main(){
	//long double* vals = collect_inputs();
	//for(int n = 0; n < 3; n++){
	//	std::cout << *(vals + n) << "\n";
	//}
	//auto buffer = new RGB_data[2000][2000];
	//(*buffer)[1][1].r = 3
	std::vector<std::vector<int>> edges;
	std::vector<std::vector<int>> map;
	std::vector<std::vector<int>> high_res_map;
	std::vector<int> filler;
	int resolutionx = 80;
	int resolutiony = 80;
	//filling low-res bitmap
	for(int i = 0; i < resolutionx; i++){
		filler.resize(resolutionx, 0);
		map.push_back(filler);
	}
	//filling high-res bitmap
	for(int i = 0; i < 1024; i++){
		filler.resize(1024, 0);
		high_res_map.push_back(filler);
	}
	char input;
	long double x = -0.765;
	long double y = 0;
	long double zoom = 0.25;
	while(true){
		std::cin >> input;
		if(input == 'h'){
			x = x - (0.08 / zoom);
		}else if(input == 'l'){
			x = x + (0.08 / zoom);
		}else if(input == 'j'){
			y = y + (0.15 / zoom);
		}else if(input == 'k'){
			y = y - (0.15 / zoom);
		}else if(input == 'z'){
			zoom = zoom / 2;
		}else if(input == 'x'){
			zoom = zoom * 2;
		}else if(input == 'p'){
			generate_image(x, y, zoom, map);
		}else if(input == 'f'){
			find_steps(x, y, zoom, 1024, 1024, high_res_map);
			edge_find(high_res_map, edges);
			find_dimension(edges);
		}
		find_steps(x, y, zoom, resolutionx, resolutiony, map);
		//edge_find(map, edges);
		//std::cout << percent_filled(map) << "\n";
	}
}
