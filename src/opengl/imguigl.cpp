daniel.wiberg@oryx.se | My favorites ▼ | Profile | Sign out

recastnavigation
Navigation-mesh Construction Toolset for Games

Project HomeDownloadsWikiIssuesSource
Checkout   Browse   Changes
Source path:  svn/  trunk/ RecastDemo/ Source/ imguiRenderGL.cpp  Edit file
‹r229
r349
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
245
246
247
248
249
250
251
252
253
254
255
256
257
258
259
260
261
262
263
264
265
266
267
268
269
270
271
272
273
274
275
276
277
278
279
280
281
282
283
284
285
286
287
288
289
290
291
292
293
294
295
296
297
298
299
300
301
302
303
304
305
306
307
308
309
310
311
312
313
314
315
316
317
318
319
320
321
322
323
324
325
326
327
328
329
330
331
332
333
334
335
336
337
338
339
340
341
342
343
344
345
346
347
348
349
350
351
352
353
354
355
356
357
358
359
360
361
362
363
364
365
366
367
368
369
370
371
372
373
374
375
376
377
378
379
380
381
382
383
384
385
386
387
388
389
390
391
392
393
394
395
396
397
398
399
400
401
402
403
404
405
406
407
408
409
410
411
412
413
414
415
416
417
418
419
420
421
422
423
424
425
426
427
428
429
430
431
432
433
434
435
436
437
438
439
440
441
442
443
444
445
446
447
448
449
450
451
452
453
454
455
456
457
458
459
460
461
462
463
464
465
466
467
468
469
470
471
472
473
474
475
476
477
478
479
480
//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include "imgui.h"
#include "SDL.h"
#include "SDL_opengl.h"

// Some math headers don't have PI defined.
static const float PI = 3.14159265f;

void imguifree(void* ptr, void* userptr);
void* imguimalloc(size_t size, void* userptr);

#define STBTT_malloc(x,y)    imguimalloc(x,y)
#define STBTT_free(x,y)      imguifree(x,y)
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

void imguifree(void* ptr, void* /*userptr*/)
{
        free(ptr);
}

void* imguimalloc(size_t size, void* /*userptr*/)
{
        return malloc(size);
}

static const unsigned TEMP_COORD_COUNT = 100;
static float g_tempCoords[TEMP_COORD_COUNT*2];
static float g_tempNormals[TEMP_COORD_COUNT*2];

static const int CIRCLE_VERTS = 8*4;
static float g_circleVerts[CIRCLE_VERTS*2];

static stbtt_bakedchar g_cdata[96]; // ASCII 32..126 is 95 glyphs
static GLuint g_ftex = 0;

inline unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
        return (r) | (g << 8) | (b << 16) | (a << 24);
}

static void drawPolygon(const float* coords, unsigned numCoords, float r, unsigned int col)
{
        if (numCoords > TEMP_COORD_COUNT) numCoords = TEMP_COORD_COUNT;

        for (unsigned i = 0, j = numCoords-1; i < numCoords; j=i++)
        {
                const float* v0 = &coords[j*2];
                const float* v1 = &coords[i*2];
                float dx = v1[0] - v0[0];
                float dy = v1[1] - v0[1];
                float d = sqrtf(dx*dx+dy*dy);
                if (d > 0)
                {
                        d = 1.0f/d;
                        dx *= d;
                        dy *= d;
                }
                g_tempNormals[j*2+0] = dy;
                g_tempNormals[j*2+1] = -dx;
        }

        for (unsigned i = 0, j = numCoords-1; i < numCoords; j=i++)
        {
                float dlx0 = g_tempNormals[j*2+0];
                float dly0 = g_tempNormals[j*2+1];
                float dlx1 = g_tempNormals[i*2+0];
                float dly1 = g_tempNormals[i*2+1];
                float dmx = (dlx0 + dlx1) * 0.5f;
                float dmy = (dly0 + dly1) * 0.5f;
                float   dmr2 = dmx*dmx + dmy*dmy;
                if (dmr2 > 0.000001f)
                {
                        float   scale = 1.0f / dmr2;
                        if (scale > 10.0f) scale = 10.0f;
                        dmx *= scale;
                        dmy *= scale;
                }
                g_tempCoords[i*2+0] = coords[i*2+0]+dmx*r;
                g_tempCoords[i*2+1] = coords[i*2+1]+dmy*r;
        }

        unsigned int colTrans = RGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, 0);

        glBegin(GL_TRIANGLES);

        glColor4ubv((GLubyte*)&col);

        for (unsigned i = 0, j = numCoords-1; i < numCoords; j=i++)
        {
                glVertex2fv(&coords[i*2]);
                glVertex2fv(&coords[j*2]);
                glColor4ubv((GLubyte*)&colTrans);
                glVertex2fv(&g_tempCoords[j*2]);

                glVertex2fv(&g_tempCoords[j*2]);
                glVertex2fv(&g_tempCoords[i*2]);

                glColor4ubv((GLubyte*)&col);
                glVertex2fv(&coords[i*2]);
        }

        glColor4ubv((GLubyte*)&col);
        for (unsigned i = 2; i < numCoords; ++i)
        {
                glVertex2fv(&coords[0]);
                glVertex2fv(&coords[(i-1)*2]);
                glVertex2fv(&coords[i*2]);
        }

        glEnd();
}

static void drawRect(float x, float y, float w, float h, float fth, unsigned int col)
{
        float verts[4*2] =
        {
                x+0.5f, y+0.5f,
                x+w-0.5f, y+0.5f,
                x+w-0.5f, y+h-0.5f,
                x+0.5f, y+h-0.5f,
        };
        drawPolygon(verts, 4, fth, col);
}

/*
static void drawEllipse(float x, float y, float w, float h, float fth, unsigned int col)
{
        float verts[CIRCLE_VERTS*2];
        const float* cverts = g_circleVerts;
        float* v = verts;

        for (int i = 0; i < CIRCLE_VERTS; ++i)
        {
                *v++ = x + cverts[i*2]*w;
                *v++ = y + cverts[i*2+1]*h;
        }

        drawPolygon(verts, CIRCLE_VERTS, fth, col);
}
*/

static void drawRoundedRect(float x, float y, float w, float h, float r, float fth, unsigned int col)
{
        const unsigned n = CIRCLE_VERTS/4;
        float verts[(n+1)*4*2];
        const float* cverts = g_circleVerts;
        float* v = verts;

        for (unsigned i = 0; i <= n; ++i)
        {
                *v++ = x+w-r + cverts[i*2]*r;
                *v++ = y+h-r + cverts[i*2+1]*r;
        }

        for (unsigned i = n; i <= n*2; ++i)
        {
                *v++ = x+r + cverts[i*2]*r;
                *v++ = y+h-r + cverts[i*2+1]*r;
        }

        for (unsigned i = n*2; i <= n*3; ++i)
        {
                *v++ = x+r + cverts[i*2]*r;
                *v++ = y+r + cverts[i*2+1]*r;
        }

        for (unsigned i = n*3; i < n*4; ++i)
        {
                *v++ = x+w-r + cverts[i*2]*r;
                *v++ = y+r + cverts[i*2+1]*r;
        }
        *v++ = x+w-r + cverts[0]*r;
        *v++ = y+r + cverts[1]*r;

        drawPolygon(verts, (n+1)*4, fth, col);
}


static void drawLine(float x0, float y0, float x1, float y1, float r, float fth, unsigned int col)
{
        float dx = x1-x0;
        float dy = y1-y0;
        float d = sqrtf(dx*dx+dy*dy);
        if (d > 0.0001f)
        {
                d = 1.0f/d;
                dx *= d;
                dy *= d;
        }
        float nx = dy;
        float ny = -dx;
        float verts[4*2];
        r -= fth;
        r *= 0.5f;
        if (r < 0.01f) r = 0.01f;
        dx *= r;
        dy *= r;
        nx *= r;
        ny *= r;

        verts[0] = x0-dx-nx;
        verts[1] = y0-dy-ny;

        verts[2] = x0-dx+nx;
        verts[3] = y0-dy+ny;

        verts[4] = x1+dx+nx;
        verts[5] = y1+dy+ny;

        verts[6] = x1+dx-nx;
        verts[7] = y1+dy-ny;

        drawPolygon(verts, 4, fth, col);
}


bool imguiRenderGLInit(const char* fontpath)
{
        for (int i = 0; i < CIRCLE_VERTS; ++i)
        {
                float a = (float)i/(float)CIRCLE_VERTS * PI*2;
                g_circleVerts[i*2+0] = cosf(a);
                g_circleVerts[i*2+1] = sinf(a);
        }

        // Load font.
        FILE* fp = fopen(fontpath, "rb");
        if (!fp) return false;
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        unsigned char* ttfBuffer = (unsigned char*)malloc(size);
        if (!ttfBuffer)
        {
                fclose(fp);
                return false;
        }

        fread(ttfBuffer, 1, size, fp);
        fclose(fp);
        fp = 0;

        unsigned char* bmap = (unsigned char*)malloc(512*512);
        if (!bmap)
        {
                free(ttfBuffer);
                return false;
        }

        stbtt_BakeFontBitmap(ttfBuffer,0, 15.0f, bmap,512,512, 32,96, g_cdata);

        // can free ttf_buffer at this point
        glGenTextures(1, &g_ftex);
        glBindTexture(GL_TEXTURE_2D, g_ftex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        free(ttfBuffer);
        free(bmap);

        return true;
}

void imguiRenderGLDestroy()
{
        if (g_ftex)
        {
                glDeleteTextures(1, &g_ftex);
                g_ftex = 0;
        }
}

static void getBakedQuad(stbtt_bakedchar *chardata, int pw, int ph, int char_index,
                                                 float *xpos, float *ypos, stbtt_aligned_quad *q)
{
        stbtt_bakedchar *b = chardata + char_index;
        int round_x = STBTT_ifloor(*xpos + b->xoff);
        int round_y = STBTT_ifloor(*ypos - b->yoff);

        q->x0 = (float)round_x;
        q->y0 = (float)round_y;
        q->x1 = (float)round_x + b->x1 - b->x0;
        q->y1 = (float)round_y - b->y1 + b->y0;

        q->s0 = b->x0 / (float)pw;
        q->t0 = b->y0 / (float)pw;
        q->s1 = b->x1 / (float)ph;
        q->t1 = b->y1 / (float)ph;

        *xpos += b->xadvance;
}

static const float g_tabStops[4] = {150, 210, 270, 330};

static float getTextLength(stbtt_bakedchar *chardata, const char* text)
{
        float xpos = 0;
        float len = 0;
        while (*text)
        {
                int c = (unsigned char)*text;
                if (c == '\t')
                {
                        for (int i = 0; i < 4; ++i)
                        {
                                if (xpos < g_tabStops[i])
                                {
                                        xpos = g_tabStops[i];
                                        break;
                                }
                        }
                }
                else if (c >= 32 && c < 128)
                {
                        stbtt_bakedchar *b = chardata + c-32;
                        int round_x = STBTT_ifloor((xpos + b->xoff) + 0.5);
                        len = round_x + b->x1 - b->x0 + 0.5f;
                        xpos += b->xadvance;
                }
                ++text;
        }
        return len;
}

static void drawText(float x, float y, const char *text, int align, unsigned int col)
{
        if (!g_ftex) return;
        if (!text) return;

        if (align == IMGUI_ALIGN_CENTER)
                x -= getTextLength(g_cdata, text)/2;
        else if (align == IMGUI_ALIGN_RIGHT)
                x -= getTextLength(g_cdata, text);

        glColor4ub(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff);

        glEnable(GL_TEXTURE_2D);

        // assume orthographic projection with units = screen pixels, origin at top left
        glBindTexture(GL_TEXTURE_2D, g_ftex);

        glBegin(GL_TRIANGLES);

        const float ox = x;

        while (*text)
        {
                int c = (unsigned char)*text;
                if (c == '\t')
                {
                        for (int i = 0; i < 4; ++i)
                        {
                                if (x < g_tabStops[i]+ox)
                                {
                                        x = g_tabStops[i]+ox;
                                        break;
                                }
                        }
                }
                else if (c >= 32 && c < 128)
                {
                        stbtt_aligned_quad q;
                        getBakedQuad(g_cdata, 512,512, c-32, &x,&y,&q);

                        glTexCoord2f(q.s0, q.t0);
                        glVertex2f(q.x0, q.y0);
                        glTexCoord2f(q.s1, q.t1);
                        glVertex2f(q.x1, q.y1);
                        glTexCoord2f(q.s1, q.t0);
                        glVertex2f(q.x1, q.y0);

                        glTexCoord2f(q.s0, q.t0);
                        glVertex2f(q.x0, q.y0);
                        glTexCoord2f(q.s0, q.t1);
                        glVertex2f(q.x0, q.y1);
                        glTexCoord2f(q.s1, q.t1);
                        glVertex2f(q.x1, q.y1);
                }
                ++text;
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);
}


void imguiRenderGLDraw()
{
        const imguiGfxCmd* q = imguiGetRenderQueue();
        int nq = imguiGetRenderQueueSize();

        const float s = 1.0f/8.0f;

        glDisable(GL_SCISSOR_TEST);
        for (int i = 0; i < nq; ++i)
        {
                const imguiGfxCmd& cmd = q[i];
                if (cmd.type == IMGUI_GFXCMD_RECT)
                {
                        if (cmd.rect.r == 0)
                        {
                                drawRect((float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
                                                 (float)cmd.rect.w*s-1, (float)cmd.rect.h*s-1,
                                                 1.0f, cmd.col);
                        }
                        else
                        {
                                drawRoundedRect((float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
                                                                (float)cmd.rect.w*s-1, (float)cmd.rect.h*s-1,
                                                                (float)cmd.rect.r*s, 1.0f, cmd.col);
                        }
                }
                else if (cmd.type == IMGUI_GFXCMD_LINE)
                {
                        drawLine(cmd.line.x0*s, cmd.line.y0*s, cmd.line.x1*s, cmd.line.y1*s, cmd.line.r*s, 1.0f, cmd.col);
                }
                else if (cmd.type == IMGUI_GFXCMD_TRIANGLE)
                {
                        if (cmd.flags == 1)
                        {
                                const float verts[3*2] =
                                {
                                        (float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f,
                                        (float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s-1, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s/2-0.5f,
                                        (float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
                                };
                                drawPolygon(verts, 3, 1.0f, cmd.col);
                        }
                        if (cmd.flags == 2)
                        {
                                const float verts[3*2] =
                                {
                                        (float)cmd.rect.x*s+0.5f, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
                                        (float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s/2-0.5f, (float)cmd.rect.y*s+0.5f,
                                        (float)cmd.rect.x*s+0.5f+(float)cmd.rect.w*s-1, (float)cmd.rect.y*s+0.5f+(float)cmd.rect.h*s-1,
                                };
                                drawPolygon(verts, 3, 1.0f, cmd.col);
                        }
                }
                else if (cmd.type == IMGUI_GFXCMD_TEXT)
                {
                        drawText(cmd.text.x, cmd.text.y, cmd.text.text, cmd.text.align, cmd.col);
                }
                else if (cmd.type == IMGUI_GFXCMD_SCISSOR)
                {
                        if (cmd.flags)
                        {
                                glEnable(GL_SCISSOR_TEST);
                                glScissor(cmd.rect.x, cmd.rect.y, cmd.rect.w, cmd.rect.h);
                        }
                        else
                        {
                                glDisable(GL_SCISSOR_TEST);
                        }
                }
        }
        glDisable(GL_SCISSOR_TEST);
}
Hide details
Change log
r248 by memononen on Oct 29, 2010   Diff
Tiny fix for imgui drawing. Implemented
adjusting of the moving target without
path finding.
Go to:
Older revisions
 r229 by memononen on Oct 10, 2010   Diff
 r213 by memononen on Aug 31, 2010   Diff
 r198 by memononen on Aug 19, 2010   Diff
All revisions of this file
File info
Size: 11172 bytes, 480 lines
View raw file
Terms - Privacy - Project Hosting Help
Powered by Google Project Hosting