#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define PI 3.14159265358979323846
#define EPSILON 1e-6

typedef struct {
  double x, y, z;
} Point;

FILE *File;

void normalize(Point *p) {
  double l = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
  p->x /= l;
  p->y /= l;
  p->z /= l;
}

int pointsEqual(Point *p1, Point *p2) {
  return(
    fabs(p1->x - p2->x) < EPSILON &&
    fabs(p1->y - p2->y) < EPSILON &&
    fabs(p1->z - p2->z) < EPSILON
  );
}

int addUniquePoint(Point *points, int *index, int max_points, Point *new_point) {
  for (int i = 0; i < *index; i++) if (pointsEqual(&points[i], new_point)) return i;

  if (*index < max_points) {
    points[*index] = *new_point;
    (*index)++;
    return *index - 1;
  }

  return -1;
}

void subdivide(Point *p1, Point *p2, Point *p3, int depth, Point *points, int *index, int max_points) {
  if (depth == 0) {
    addUniquePoint(points, index, max_points, p1);
    addUniquePoint(points, index, max_points, p2);
    addUniquePoint(points, index, max_points, p3);
    return;
  }

  Point p12 = {(p1->x + p2->x) / 2, (p1->y + p2->y) / 2, (p1->z + p2->z) / 2}; normalize(&p12);
  Point p23 = {(p2->x + p3->x) / 2, (p2->y + p3->y) / 2, (p2->z + p3->z) / 2}; normalize(&p23);
  Point p31 = {(p3->x + p1->x) / 2, (p3->y + p1->y) / 2, (p3->z + p1->z) / 2}; normalize(&p31);

  subdivide(p1, &p12, &p31, depth - 1, points, index, max_points);
  subdivide(p2, &p23, &p12, depth - 1, points, index, max_points);
  subdivide(p3, &p31, &p23, depth - 1, points, index, max_points);
  subdivide(&p12, &p23, &p31, depth - 1, points, index, max_points);
}

void createGeodesicSphere(int subdivisions, Point *points, int *points_count, int max_points) {
  Point icosahedron[12] = {
    {0, 1, 0},
    {0.894427, 0.447214, 0},
    {0.276393, 0.447214, 0.850651},
    {-0.723607, 0.447214, 0.525731},
    {-0.723607, 0.447214, -0.525731},
    {0.276393, 0.447214, -0.850651},
    {0.723607, -0.447214, 0.525731},
    {-0.276393, -0.447214, 0.850651},
    {-0.894427, -0.447214, 0},
    {-0.276393, -0.447214, -0.850651},
    {0.723607, -0.447214, -0.525731},
    {0, -1, 0}
  };

  int faces[20][3] = {
    {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5},
    {0, 5, 1}, {11, 6, 7}, {11, 7, 8}, {11, 8, 9},
    {11, 9, 10}, {11, 10, 6}, {1, 6, 2}, {2, 7, 3},
    {3, 8, 4}, {4, 9, 5}, {5, 10, 1}, {6, 7, 2},
    {7, 8, 3}, {8, 9, 4}, {9, 10, 5}, {10, 6, 1}
  };

  int index = 0;
  for (int i = 0; i < 20; i++)
    subdivide(&icosahedron[faces[i][0]], &icosahedron[faces[i][1]], &icosahedron[faces[i][2]], subdivisions, points, &index, max_points);
  
  *points_count = index;
}

int main(int argc, char *argv[]) {
  if (argc != 8) return printf("usage: %s <subdivisions> <radius> <offset x> <offset y> <offset z> <particle> <output file>", argv[0]);
  
  int points_count = 0;

  int subdivisions = atoi(argv[1]);
  float radius = atof(argv[2]);
  float off_x = atof(argv[3]);
  float off_y = atof(argv[4]);
  float off_z = atof(argv[5]);
  char *particle = argv[6];
  char *f = argv[7];

  int max_points = 10 * pow(4, subdivisions) + 2;
  Point *points = (Point *) malloc(max_points * sizeof(Point));

  if (!points) return printf("Malloc failed.\n");

  createGeodesicSphere(subdivisions, points, &points_count, max_points);

  File = fopen(f, "w");
  for (int i = 0; i < points_count; i++) 
    fprintf(File, "particle %s ~%lf ~%lf ~%lf 0 0 0 0 1\n",
        particle,
        points[i].x * radius + off_x,
        points[i].y * radius + off_y,
        points[i].z * radius + off_z
    );

  fclose(File);
  free(points);
}