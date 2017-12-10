#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct point point;
typedef struct point_node point_node;


int swap(point * p1, point * p2);
int dist(point * p1, point * p2);
int angle(point *p0, point * p1, point * p2);
int compare(point * p0, point * p1, point * p2); //compare the angle of two points and p0, if they are colinear with p0, pick the furthest one from p0
point_node * convex_hull(point * points[], int n);
void print_array(point * points[], int n);
int min_point(point * points[], int n);
void quicksort_polar(point * points[], int q, int r); //quicksort by slope formed by p0 and the points, from least positive slope to least negative slope
int partition(point * points[], int p, int r); //implements slope comparision for quicksort
point ** create_array(int n);
void add_to_list(point_node * head, point * new_point); //adds point node to list immediately after given point node (head), using given point data
void print_list(point_node * head);
void free_array(point ** points, int n);
void free_list(point_node * head);
void delete_node(point_node* node);
void write_to_file(point * points[], int n, point_node* head);

struct point
{
	int x;
	int y;
};

struct point_node //linked list version of point
{
	int x;
	int y;
	point_node * next_;
	point_node * prev_;
};

int main(void) {
	int x = 0;
	int y = 0;
	int n = 0;

    scanf("%d\n", &n);//get number of points, N

    point ** points = create_array(n);//calloc(n, sizeof(point*));
    for (int i = 0; i < n; i++) {
        scanf("%d %d\n", &x, &y);
        points[i]->x = x;
        points[i]->y = y;
    }
    point_node * result = convex_hull(points, n);
    write_to_file(points, n, result);

    free_array(points, n);
    free_list(result);
    printf("Done! Results placed in results.txt\n");
}

//Swap two points from a list
int swap(point * p1, point * p2) {
	point temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

//Calculate distance between two points
int dist(point * p1, point * p2) {
	int dist = (pow(p1->x - p2->x, 2))+(pow(p1->y - p2->y, 2));
	return dist;
}

//Calculate Angle difference between two points using p0
int angle(point * p0, point * p1, point * p2) {
	int angle_val = ((p1->y - p0->y) * (p2->x - p1->x)) - ((p1->x - p0->x) * (p2->y - p1->y));
	if (angle_val == 0) {//angle is equal from p0
		return 0;
	}
	return (angle_val > 0) ? 1 : 2; //2 for angle is counterclockwise from p0, 1 for angle is clockwise from p0
}

//compare the angle of two points and p0, if they are colinear with p0, pick the furthest one from p0
int compare(point * p0, point * p1, point* p2) {
	int angle_var = angle(p0, p1, p2);
	if (angle_var == 0) {
		return (dist(p0, p2) >= dist(p0, p1)) ? -1 : 1;
	}
	if (angle_var == 2) {
		return -1;
	}
	return 1;
}

//Sort array of points and make a new list containing only convex hull
point_node * convex_hull(point * points[], int n) {
	int min = min_point(points, n);
	swap(points[0], points[min]);
	point * p0 = points[0];
	quicksort_polar(points, 1, n-1);

	int m = 1;
	for (int i = 1; i < n; i++) {
		while (i < n - 1 && angle(p0, points[i], points[i + 1]) == 0) {
			i++;
		}
		swap(points[m], points[i]);
		m++;
	}
	if (m < 3)
	{
		printf("Less than three points in Convex Hull, exiting...\n ");
		exit(1);
	}

	point_node * head = calloc(1, sizeof(point_node));
	head->x = points[0]->x;
	head->y = points[0]->y;
	add_to_list(head, points[1]);
	add_to_list(head->next_, points[2]);
	point_node * last = head->next_->next_;
	for (int i = 3; i < m; i++) {//place values in result array
		while (angle(last->prev_, last, points[i]) != 2) {
			last = last->prev_;
			delete_node(last->next_);
		}

		add_to_list(last, points[i]);
		last = last->next_;
	}
	return head;

}

void print_array(point * points[], int n) {
	for (int i = 0; i < n; i++) {
		printf("(%d, %d)\n", points[i]->x, points[i]->y);
	}
}

//Find the point with the lowest value for x and y, this will be p0
int min_point(point * points[], int n) {
	int min_point = 0;
	int min_val = points[0]->y;
	for (int i = 1; i < n; i++) {
		if (points[i]->y <= min_val) {
			min_val = points[i]->y;
			if (points[i]->y == points[min_point]->y) {
				if (points[i]->x < points[min_point]->x) {
					min_point = i;
				}
			}
			else {
				min_point = i;
			}
		}
	}
	return min_point;

}

//quicksort by slope formed by p0 and the points, from least positive slope to least negative slope
void quicksort_polar(point * points[], int p, int r) {
	int q = 0;
	if (p < r) {
		q = partition(points, p, r);
		quicksort_polar(points, p, q - 1);
		quicksort_polar(points, q + 1, r);

	}
}

//implements slope comparision for quicksort
int partition(point * points[], int p, int r) {
	int x = r;
	int i = p - 1;
	for (int j = p; j <= r - 1; j++) {
		if (compare(points[0], points[x], points[j]) == 1) {
			i++;
            swap(points[i], points[j]);
		}
	}
	swap(points[i + 1], points[r]);
	return i + 1;
}

/*Helper Functions*/

point ** create_array(int n) {
	point ** array = calloc(n, sizeof(point*));
	for (int j = 0; j < n; j++) {
		point * new_point = calloc(1, sizeof(point));
		array[j] = new_point;
	}
	return array;
}

//adds point node to list immediately after given point node (head), using given point data
void add_to_list(point_node * head, point * new_point) {

	if (head == NULL) {
		return;
	}
	point_node* temp = head;
	point_node* new_point_node = calloc(1, sizeof(point_node));
	new_point_node->x = new_point->x;
	new_point_node->y = new_point->y;
	head->next_ = new_point_node;
	head->next_->prev_ = temp;
}

void print_list(point_node * head) {
	while (head != NULL) {
		printf("(%d, %d)\n", head->x, head->y);
		head = head->next_;
	}
}

void free_array(point ** points, int n) {
	for (int i = 0; i < n; i++) {
            free(points[i]);
	}
	free(points);
}
void free_list(point_node * head) {
	while (head->next_ != NULL) {
		head = head->next_;
		delete_node(head->prev_);
	}
	delete_node(head);
}

void delete_node(point_node* node) {
	if (node == NULL) {
		return;
	}
	if (node->prev_ != NULL) {
		node->prev_->next_ = node->next_;

	}
	if (node->next_ != NULL) {
		node->next_->prev_ = node->prev_;
	}
	free(node);
}

void write_to_file(point * points[], int n, point_node* head) {
	FILE *fp;
	fp = fopen("results.txt", "w");
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%d ", points[i]->x);
	}
	fprintf(fp, "\n");
	for (int i = 0; i < n; i++) {
		fprintf(fp, "%d ", points[i]->y);
	}
	fprintf(fp, "\n");
	//print result
	point_node * temp_head = head;
	while (temp_head != NULL) {
		fprintf(fp, "%d ", temp_head->x);
		temp_head = temp_head->next_;
	}
	fprintf(fp, "%d ", head->x);
	fprintf(fp, "\n");
	temp_head = head;
	while (temp_head != NULL) {
		fprintf(fp, "%d ", temp_head->y);
		temp_head = temp_head->next_;
	}
	fprintf(fp, "%d ", head->y);
	fprintf(fp, "\n");
	fclose(fp);

}
