#ifndef ___GLINC_H
#define ___GLINC_H

#ifdef HW_ENABLE_GLES

#include <GLES/egl.h>
#include <GLES/glext.h>

#define GL_QUADS                0x0007
#define GL_POLYGON              0x0009

static unsigned int gles_immediate = 0;
static GLenum gles_mode;
static GLfloat gles_vertex[16384];
static GLfloat gles_texcoord[16384];
static GLfloat gles_colors[16384];
static GLfloat gles_normals[16384];
static unsigned int gles_vertex_count = 0;
static unsigned int gles_texcoord_count = 0;
static unsigned int gles_color_count = 0;
static unsigned int gles_normal_count = 0;
static unsigned char gles_vertex_dimensions = 2;
static GLushort gles_quad_indices[4] = { 1, 2, 0, 3 };

static inline void gles_vertex_data(void) {
    if (!gles_normal_count) gles_normal_count = 3;
    while (gles_normal_count / 3 < gles_vertex_count / gles_vertex_dimensions) {
        gles_normals[gles_normal_count] = gles_normals[gles_normal_count - 3];
        gles_normal_count++;
        gles_normals[gles_normal_count] = gles_normals[gles_normal_count - 3];
        gles_normal_count++;
        gles_normals[gles_normal_count] = gles_normals[gles_normal_count - 3];
        gles_normal_count++;
    }
    if (!gles_color_count) gles_color_count = 4;
    while (gles_color_count / 4 < gles_vertex_count / gles_vertex_dimensions) {
        gles_colors[gles_color_count] = gles_colors[gles_color_count - 4];
        gles_color_count++;
        gles_colors[gles_color_count] = gles_colors[gles_color_count - 4];
        gles_color_count++;
        gles_colors[gles_color_count] = gles_colors[gles_color_count - 4];
        gles_color_count++;
        gles_colors[gles_color_count] = gles_colors[gles_color_count - 4];
        gles_color_count++;
    }
}

static inline void gles_render_current(void) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(gles_vertex_dimensions, GL_FLOAT, 0, gles_vertex);
    if (gles_texcoord_count && gles_mode >= GL_TRIANGLES) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, gles_texcoord);
    }
    if (gles_color_count) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, gles_colors);
    }
    if (gles_normal_count && gles_mode >= GL_TRIANGLES) {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, gles_normals);
    }
    switch (gles_mode) {
        case GL_POINTS:
        case GL_LINES:
        case GL_LINE_LOOP:
        case GL_LINE_STRIP:
        case GL_TRIANGLES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
            glDrawArrays(gles_mode, 0, gles_vertex_count / gles_vertex_dimensions);
            break;
        case GL_QUADS:
            glDrawElements(GL_TRIANGLE_STRIP, gles_vertex_count / gles_vertex_dimensions, GL_UNSIGNED_SHORT, gles_quad_indices);
            break;
        case GL_POLYGON: {
            unsigned int i;
            GLfloat center_x = 0.0f, center_y = 0.0f;
            gles_vertex[gles_vertex_count++] = gles_vertex[0];
            gles_vertex[gles_vertex_count++] = gles_vertex[1];
            GLushort poly_indices[(gles_vertex_count / gles_vertex_dimensions) + 1];
            poly_indices[0] = gles_vertex_count / gles_vertex_dimensions;
            for (i = 0; i < gles_vertex_count / gles_vertex_dimensions; i++) {
                poly_indices[i + 1] = i;
                center_x += gles_vertex[i * 2] / (gles_vertex_count / gles_vertex_dimensions);
                center_y += gles_vertex[(i * 2) + 1] / (gles_vertex_count / gles_vertex_dimensions);
            }
            gles_vertex[gles_vertex_count++] = center_x;
            gles_vertex[gles_vertex_count++] = center_y;
            gles_vertex_data();
            glDrawElements(GL_TRIANGLE_FAN, gles_vertex_count / gles_vertex_dimensions, GL_UNSIGNED_SHORT, poly_indices);
            break;
        }
        default:
            printf("gles_render_current: unsupported mode: 0x%x\n", gles_mode);
            break;
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    gles_vertex_count = 0;
    gles_texcoord_count = 0;
    if (gles_color_count) {
        gles_colors[0] = gles_colors[gles_color_count - 4];
        gles_colors[1] = gles_colors[gles_color_count - 3];
        gles_colors[2] = gles_colors[gles_color_count - 2];
        gles_colors[3] = gles_colors[gles_color_count - 1];
    }
    gles_color_count = 0;
    if (gles_normal_count) {
        gles_normals[0] = gles_normals[gles_normal_count - 3];
        gles_normals[1] = gles_normals[gles_normal_count - 2];
        gles_normals[2] = gles_normals[gles_normal_count - 1];
    }
    gles_normal_count = 0;
}

static inline void glBegin(GLenum mode) {
    gles_immediate = 1;
    gles_mode = mode;
}

static inline void glEnd(void) {
    if (gles_vertex_count) gles_render_current();
    gles_immediate = 0;
}

static inline void glesColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    if (gles_immediate) {
        if (gles_color_count / 4 <= gles_vertex_count / gles_vertex_dimensions) {
            gles_colors[gles_color_count++] = red;
            gles_colors[gles_color_count++] = green;
            gles_colors[gles_color_count++] = blue;
            gles_colors[gles_color_count++] = alpha;
        } else {
            gles_colors[gles_color_count - 4] = red;
            gles_colors[gles_color_count - 3] = green;
            gles_colors[gles_color_count - 2] = blue;
            gles_colors[gles_color_count - 1] = alpha;
        }
    } else {
        gles_colors[0] = red;
        gles_colors[1] = green;
        gles_colors[2] = blue;
        gles_colors[3] = alpha;
        glColor4f(red, green, blue, alpha);
    }
}

static inline void glColor3f(GLfloat red, GLfloat green, GLfloat blue) {
    glesColor4f(red, green, blue, 1.0f);
}

static inline void glesColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) {
    glesColor4f((GLfloat)red / 255.0f, (GLfloat)green / 255.0f, (GLfloat)blue / 255.0f, (GLfloat)alpha / 255.0f);
}

static inline void glColor3ub(GLubyte red, GLubyte green, GLubyte blue) {
    glesColor4ub(red, green, blue, 255);
}

static inline void glesNormal3f(GLfloat nx, GLfloat ny, GLfloat nz) {
    if (gles_immediate) {
        if (gles_normal_count / 3 <= gles_vertex_count / gles_vertex_dimensions) {
            gles_normals[gles_normal_count++] = nx;
            gles_normals[gles_normal_count++] = ny;
            gles_normals[gles_normal_count++] = nz;
        } else {
            gles_normals[gles_normal_count - 3] = nx;
            gles_normals[gles_normal_count - 2] = ny;
            gles_normals[gles_normal_count - 1] = nz;
        }
    } else {
        gles_normals[0] = nx;
        gles_normals[1] = ny;
        gles_normals[2] = nz;
        glNormal3f(nx, ny, nz);
    }
}

static inline void glNormal3fv(const GLfloat *v) {
    glesNormal3f(v[0], v[1], v[2]);
}

static inline void glTexCoord2f(GLfloat s, GLfloat t) {
    gles_texcoord[gles_texcoord_count++] = s;
    gles_texcoord[gles_texcoord_count++] = t;
}

static inline void glVertex2f(GLfloat x, GLfloat y) {
    if (gles_mode == GL_QUADS && gles_vertex_dimensions && gles_vertex_count / gles_vertex_dimensions == 4) gles_render_current();
    gles_vertex_dimensions = 2;
    gles_vertex[gles_vertex_count++] = x;
    gles_vertex[gles_vertex_count++] = y;
    gles_vertex_data();
}

static inline void glVertex3f(GLfloat x, GLfloat y, GLfloat z) {
    if (gles_mode == GL_QUADS && gles_vertex_dimensions && gles_vertex_count / gles_vertex_dimensions == 4) gles_render_current();
    gles_vertex_dimensions = 3;
    gles_vertex[gles_vertex_count++] = x;
    gles_vertex[gles_vertex_count++] = y;
    gles_vertex[gles_vertex_count++] = z;
    gles_vertex_data();
}

static inline void glVertex3fv(const GLfloat *v) {
    glVertex3f(v[0], v[1], v[2]);
}

#define glColor4ub(R, G, B, A) glesColor4ub(R, G, B, A)
#define glColor4f(R, G, B, A) glesColor4f(R, G, B, A)
#define glNormal3f(X, Y, Z) glesNormal3f(X, Y, Z)

#else

#include <SDL_opengl.h>

extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

#endif

int glCheckExtension(const char *ext);

#endif
