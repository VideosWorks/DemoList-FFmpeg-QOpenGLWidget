/*\
 * geometry.c - functions that define specific geometry (e.g., the asteroid model)
 * Asteroid: a modern version of the arcade classic Asteroids
 *           <https://github.com/chazomaticus/asteroid>
 * Copyright (C) 2003, 2005, 2012, 2013 Charles Lindsay <chaz@chazomatic.us>
 *
 * Asteroids is originally copyright (C) 1979 Atari, Inc.
 *
 * Asteroid is released under the GNU GPL and as such, comes with absolutely no
 * warranty of any kind, whatsoever.  See COPYING for more information.
\*/


#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "main.h"
#include "render.h"
#include "geometry.h"


void ComputeNormals(const VECTOR3 *, const unsigned int *, unsigned int, VECTOR3 *, unsigned int); //computes normals in a vertex array


VECTOR3 asteroid_vertices[] = //an oblong 1-unit 'sphere'
{                             //whose positions will be randomly perturbed
   { 0.0f,  0.0f,  0.5f },
   { 0.3f,  0.3f,  0.25f},
   {-0.3f,  0.3f,  0.25f},
   {-0.3f, -0.3f,  0.25f},
   { 0.3f, -0.3f,  0.25f},
   { 0.5f,  0.0f,  0.0f },
   { 0.0f,  0.5f,  0.0f },
   {-0.5f,  0.0f,  0.0f },
   { 0.0f, -0.5f,  0.0f },
   { 0.3f,  0.3f, -0.25f},
   {-0.3f,  0.3f, -0.25f},
   {-0.3f, -0.3f, -0.25f},
   { 0.3f, -0.3f, -0.25f},
   { 0.0f,  0.0f, -0.5f },
};
unsigned int asteroid_indices[] = //the order they need to be drawn to
{                                 //make the vertices a sphere
    0,  1,  2,
    0,  2,  3,
    0,  3,  4,
    0,  4,  1,
    1,  5,  6,
    1,  6,  2,
    2,  6,  7,
    2,  7,  3,
    3,  7,  8,
    3,  8,  4,
    4,  8,  5,
    4,  5,  1,
    5,  9,  6,
    6,  9, 10,
    6, 10,  7,
    7, 10, 11,
    7, 11,  8,
    8, 11, 12,
    8, 12,  5,
    5, 12,  9,
   13,  9, 12,
   13, 10,  9,
   13, 11, 10,
   13, 12, 11,
};
#define num_asteroid_vertices (sizeof(asteroid_vertices) / sizeof(asteroid_vertices[0]))
#define asteroid_triangles ((sizeof(asteroid_indices) / sizeof(asteroid_indices[0])) / 3)
VECTOR3 asteroid_normals[num_asteroid_vertices];
VECTOR2 asteroid_texcoords[asteroid_triangles * 3] =
{
   {0.0f  , 1.0f  }, {0.375f, 0.875f}, {0.5f  , 1.0f  },
   {0.0f  , 1.0f  }, {0.0f  , 0.5f  }, {0.125f, 0.625f},
   {0.0f  , 1.0f  }, {0.125f, 0.625f}, {0.25f , 0.75f },
   {0.0f  , 1.0f  }, {0.25f , 0.75f }, {0.375f, 0.875f},
   {0.375f, 0.875f}, {0.75f , 0.75f }, {1.0f  , 1.0f  },
   {0.375f, 0.875f}, {1.0f  , 1.0f  }, {0.5f  , 1.0f  },
   {0.0f  , 0.5f  }, {0.0f  , 0.0f  }, {0.25f , 0.25f },
   {0.0f  , 0.5f  }, {0.25f , 0.25f }, {0.125f, 0.625f},
   {0.125f, 0.625f}, {0.25f , 0.25f }, {0.5f  , 0.5f  },
   {0.125f, 0.625f}, {0.5f  , 0.5f  }, {0.25f , 0.75f },
   {0.25f , 0.75f }, {0.5f  , 0.5f  }, {0.75f , 0.75f },
   {0.25f , 0.75f }, {0.75f , 0.75f }, {0.375f, 0.875f},
   {0.75f , 0.75f }, {0.875f, 0.375f}, {1.0f  , 1.0f  },
   {1.0f  , 1.0f  }, {0.875f, 0.375f}, {1.0f  , 0.5f  },
   {0.0f  , 0.0f  }, {0.5f  , 0.0f  }, {0.25f , 0.25f },
   {0.25f , 0.25f }, {0.5f  , 0.0f  }, {0.625f, 0.125f},
   {0.25f , 0.25f }, {0.625f, 0.125f}, {0.5f  , 0.5f  },
   {0.5f  , 0.5f  }, {0.625f, 0.125f}, {0.75f , 0.25f },
   {0.5f  , 0.5f  }, {0.75f , 0.25f }, {0.75f , 0.75f },
   {0.75f , 0.75f }, {0.75f , 0.25f }, {0.875f, 0.375f},
   {1.0f  , 0.0f  }, {0.875f, 0.375f}, {0.75f , 0.25f },
   {1.0f  , 0.0f  }, {1.0f  , 0.5f  }, {0.875f, 0.375f},
   {1.0f  , 0.0f  }, {0.625f, 0.125f}, {0.5f  , 0.0f  },
   {1.0f  , 0.0f  }, {0.75f , 0.25f }, {0.625f, 0.125f},
};

VECTOR3 playership_vertices[] = //the vertices that make up the player ship
{
   { 0.5f,  0.0f ,  0.0f },
   {-0.4f,  0.0f ,  0.3f },
   {-0.4f, -0.26f, -0.15f},
   {-0.4f,  0.26f, -0.15f},
   {-0.5f,  0.0f ,  0.0f },
};
unsigned int playership_indices[] = //the draw order of the player ship vertices
{
   0, 1, 2,
   0, 2, 3,
   0, 3, 1,
   2, 1, 4,
   3, 2, 4,
   1, 3, 4,
};
#define num_playership_vertices (sizeof(playership_vertices) / sizeof(playership_vertices[0]))
#define playership_triangles ((sizeof(playership_indices) / sizeof(playership_indices[0])) / 3)
VECTOR3 playership_normals[playership_triangles];
VECTOR3 playership_colors[num_playership_vertices] =
{
   {1.0f , 1.0f , 0.25f},
   {0.75f, 0.75f, 0.25f},
   {0.75f, 0.75f, 0.25f},
   {0.75f, 0.75f, 0.25f},
   {1.0f , 0.0f , 0.0f },
};

VECTOR3 alienship_vertices[] = //the vertices that make the alien ship
{
   { 0.0f  ,  0.0f ,  0.1f},
   { 0.0f  , -0.1f ,  0.0f},
   {-0.087f,  0.05f,  0.0f},
   { 0.087f,  0.05f,  0.0f},
   { 0.0f  , -0.5f , -0.1f},
   {-0.433f,  0.25f, -0.1f},
   { 0.433f,  0.25f, -0.1f},
};
unsigned int alienship_indices[] = //the draw order of the alien ship vertices
{
   0, 4, 1,
   0, 1, 5,
   0, 5, 2,
   0, 2, 6,
   0, 6, 3,
   0, 3, 4,
   1, 4, 3,
   2, 5, 1,
   3, 6, 2,
   3, 2, 1,
};
#define num_alienship_vertices (sizeof(alienship_vertices) / sizeof(alienship_vertices[0]))
#define alienship_triangles ((sizeof(alienship_indices) / sizeof(alienship_indices[0])) / 3)
VECTOR3 alienship_normals[num_alienship_vertices];
VECTOR3 alienship_colors[num_alienship_vertices] =
{
   {0.125f, 0.75f , 0.25f },
   {0.125f, 0.75f , 0.25f },
   {0.125f, 0.75f , 0.25f },
   {0.125f, 0.75f , 0.25f },
   {0.875f, 0.125f, 0.125f},
   {0.875f, 0.125f, 0.125f},
   {0.875f, 0.125f, 0.125f},
};


//initializes all geometry rendered by the game
void InitGeometry(void)
{
   int i, j;

   //randomly perturb vertices
   for(i = 0; i < sizeof(asteroid_vertices) / sizeof(asteroid_vertices[0]); i++)
   {
      for(j = 0; j < 3; j++)
         asteroid_vertices[i].v[j] += (float)rand() / ((float)RAND_MAX * 4.0f) - 0.125f; //add rand in range [-0.25, 0.25] to each position
   }

   //make normals
   ComputeNormals(asteroid_vertices, asteroid_indices, asteroid_triangles, asteroid_normals, num_asteroid_vertices);
   ComputeNormals(playership_vertices, playership_indices, playership_triangles, playership_normals, 0);
   ComputeNormals(alienship_vertices, alienship_indices, alienship_triangles, alienship_normals, num_alienship_vertices);
}

//creates an asteroid model
MODEL MakeAsteroid(TEXTURE texture)
{
   return CreateModel(asteroid_vertices, asteroid_indices, asteroid_triangles, asteroid_normals, 1, NULL, asteroid_texcoords, &texture);
}

//creates a player ship model
MODEL MakePlayerShip(void)
{
   return CreateModel(playership_vertices, playership_indices, playership_triangles, playership_normals, 0, playership_colors, NULL, NULL);
}

//creates an alien ship model
MODEL MakeAlienShip(void)
{
   return CreateModel(alienship_vertices, alienship_indices, alienship_triangles, alienship_normals, 1, alienship_colors, NULL, NULL);
}

//calculates the normals for each vertex
void ComputeNormals(const VECTOR3 * vertices, const unsigned int * indices, unsigned int triangles, VECTOR3 * normals_out, unsigned int n_vertices_if_averaging)
{
   unsigned int i, j, k, refs;
   float mag;
   VECTOR3 d01, d12; //difference vectors between first and second, and second and third vertices of a face

   VECTOR3 * t_normals = (VECTOR3 *)ast_alloc(sizeof(VECTOR3) * triangles);

   for(i = 0; i < triangles; i++)
   {
      for(j = 0; j < 3; j++)
      {
         d01.v[j] = vertices[indices[i * 3 + 1]].v[j]  //d01[x,y,z] = vertex[i + 1][x,y,z] - vertex[i + 0][x,y,z]
                  - vertices[indices[i * 3 + 0]].v[j];
         d12.v[j] = vertices[indices[i * 3 + 2]].v[j]  //d12[x,y,z] = vertex[i + 2][x,y,z] - vertex[i + 1][x,y,z]
                  - vertices[indices[i * 3 + 1]].v[j];
      }

      t_normals[i].x = d01.y * d12.z - d01.z * d12.y; //n[x] = d01[y]*d12[z] - d01[z]*d12[y]
      t_normals[i].y = d01.z * d12.x - d01.x * d12.z; //n[y] = d01[z]*d12[x] - d01[x]*d12[z]
      t_normals[i].z = d01.x * d12.y - d01.y * d12.x; //n[z] = d01[x]*d12[y] - d01[y]*d12[x]
   }

   if(n_vertices_if_averaging) //if we're to calculate vertex normals from the plane normals
   {
      for(i = 0; i < triangles; i++) //normalize the plane normals manually so average works out correctly
      {
         mag = (float)sqrt(t_normals[i].x*t_normals[i].x + t_normals[i].y*t_normals[i].y + t_normals[i].z*t_normals[i].z);

         for(j = 0; j < 3; j++)
            t_normals[i].v[j] /= mag;
      }

      for(i = 0; i < n_vertices_if_averaging; i++) //for each vertex
      {
         refs = 0; //no references to it yet
         for(j = 0; j < 3; j++) //clear the normal to 0
            normals_out[i].v[j] = 0.0f;

         for(j = 0; j < triangles * 3; j++) //find every reference to it in index buffer
         {
            if(indices[j] == i) //if this vertex is referenced in index buffer
            {
               refs++; //one more reference
               for(k = 0; k < 3; k++) //add the corresponding normal into the vertex normal
                  normals_out[i].v[k] += t_normals[j / 3].v[k];
            }
         }

         if(refs) //if it was referenced (should always be)
         {
            for(j = 0; j < 3; j++)
               normals_out[i].v[j] /= (float)refs; //take average of all referencing normals
         }
      }
   }
   else //normals_out should hold plane normals
      memcpy(normals_out, t_normals, sizeof(VECTOR3) * triangles);

   free(t_normals);
}
