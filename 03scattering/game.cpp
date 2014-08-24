#include "game.h"

int 
	window_width,
	window_height;
Mesh 
	mesh_teapot,
	mesh_quad;
Shader 
	shader_texture,
	shader_depth,
	shader_scattering;
mat4
	mat_projection,
	mat_projection_light,
	mat_view_light,
	mat_view;

RenderTexture rt_depth;

bool load_game(int width, int height)
{
	window_width = width;
	window_height = height;

	if (!shader_texture.load_from_file("./texture.vs", "./texture.fs") ||
		!shader_depth.load_from_file("./depth.vs", "./depth.fs") ||
		!shader_scattering.load_from_file("./scattering.vs", "./scattering.fs"))
		return false;
	if (!load_mesh_indexed(mesh_teapot, "../data/models/teapot.obj"))
		return false;

	mesh_quad = gen_tex_quad();
	return true;
}

void free_game()
{
	
}

void init_game()
{
	rt_depth = gen_rendertexture(window_width * 2, window_height * 2, GL_R32F);
	mat_view = mat4(1.0f);
}

float z_near = 0.1f;
float z_far = 6.0f;
vec3 light_pos;
void update_game(float dt)
{
	if (is_key_down('z')) z_near += dt;
	else if (is_key_down('x')) z_near -= dt;
	if (is_key_down('a')) z_far += dt;
	else if (is_key_down('s')) z_far -= dt;

	APP_LOG << "\rnear: " << z_near << "\tfar: " << z_far << "\t\t";

	float t = get_elapsed_time();
	mat_projection_light = perspective(PI / 4.0f, window_width / float(window_height), z_near, z_far);
	mat_view_light = translate(0.0f, 0.0f, -2.0f) * rotateX(-0.43f);

	mat_projection = perspective(PI / 4.0f, window_width / float(window_height), z_near, z_far);
	mat_view = translate(0.0f, 0.0f, -2.0f) * rotateX(-0.43f) * rotateY(t);

	light_pos = vec3(glm::inverse(mat_view_light) * vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void render_game(float dt)
{
	mat4 mat_model = scale(0.07f);

	// Render depth map

	glBindFramebuffer(GL_FRAMEBUFFER, rt_depth.fbo);
	glViewport(0, 0, rt_depth.width, rt_depth.height);

	depth_test(true, GL_LEQUAL);
	depth_write(true);
	clear(0x00000000, 1.0f);

	use_shader(shader_depth);
	uniform("projection", mat_projection_light);
	uniform("view", mat_view_light);
	uniform("model", mat_model);
	array_buffer(mesh_teapot.vertex_buffer);
	element_buffer(mesh_teapot.index_buffer);
	attribfv("position", 3, 8, 0);
	attribfv("normal", 3, 8, 5);
	glDrawElements(GL_TRIANGLES, mesh_teapot.num_indices, GL_UNSIGNED_INT, 0);

	// Render the geometry

	glBindTexture(GL_TEXTURE_2D, rt_depth.color);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window_width, window_height);

	clear(0x00000000, 1.0f);

	use_shader(shader_scattering);
	uniform("depthMap", 0);
	uniform("lightPos", light_pos);
	uniform("lightMatrix", mat_projection_light * mat_view_light);
	uniform("projection", mat_projection);
	uniform("view", mat_view);
	uniform("model", mat_model);
	array_buffer(mesh_teapot.vertex_buffer);
	element_buffer(mesh_teapot.index_buffer);
	attribfv("position", 3, 8, 0);
	attribfv("normal", 3, 8, 5);
	glDrawElements(GL_TRIANGLES, mesh_teapot.num_indices, GL_UNSIGNED_INT, 0);

	//use_shader(shader_texture);
	//uniform("tex", 0);
	//mesh_quad.draw();
}

void on_key_up(uint16 mod, SDL_Keycode key) { }
void on_key_down(uint16 mod, SDL_Keycode key) { }
void on_mouse_moved(int x, int y, int dx, int dy) { }
void on_mouse_dragged(int button, int x, int y, int dx, int dy) { }
void on_mouse_pressed(int button, int x, int y) { }
void on_mouse_released(int button, int x, int y) { }