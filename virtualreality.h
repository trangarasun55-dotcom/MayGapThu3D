#ifndef VIRTUAL_REALITY_H
#define VIRTUAL_REALITY_H

#define _USE_MATH_DEFINES

//
// Ky thuat do hoa
// Ha Manh Toan lhttuy@gmail.com
//
#include <glut.h>
#include <vector>
#include <math.h>

#include <unordered_map>
#include <iostream>
#include <fstream>

namespace hmtoan {

template<typename T>
void print(const char* name, const T& v) {
	std::cout << name << v << "\n";
}
// =================== Math ===================
#define DEG2RAD M_PI/180.0
#define RAD2DEG 180.0/M_PI
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

template <typename T>
T clamp(T p, T mi, T ma) {
	if (p < mi) return mi;
	else if (p > ma) return ma;
	else return p;
}

struct Vec2 {
	float x = 0, y = 0;
	Vec2(float x = 0, float y = 0) : x(x), y(y){}
};

struct Vec3 {
	float x = 0, y = 0, z = 0;
	Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};
inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}
inline float length(Vec3 a) {
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}
inline float dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline Vec3 operator+(Vec3 a, Vec3 b) {
	Vec3 v;
	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return v;
}

inline Vec3 operator-(Vec3 a, Vec3 b) {
	Vec3 v;
	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	return v;
}
inline Vec3& operator+=(Vec3& a, const Vec3& b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}
inline Vec3 operator*(const Vec3& v, float s) {
	return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator*(float s, const Vec3& v) {
	return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 min(const Vec3& a, const Vec3& b) {
	return {
		MIN(a.x, b.x),
		MIN(a.y, b.y),
		MIN(a.z, b.z)
	};
}

inline Vec3 max(const Vec3& a, const Vec3& b) {
	return {
		MAX(a.x, b.x),
		MAX(a.y, b.y),
		MAX(a.z, b.z)
	};
}
inline Vec3 cross(const Vec3& a, const Vec3& b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}
inline Vec3 normalize(const Vec3& v) {
	float len = length(v);
	if (len < 1e-6f) return { 0,0,0 };
	return v * (1.0f / len);
}
inline Vec3 computeFaceNormal(const Vec3& a, const Vec3& b, const Vec3& c) {
	Vec3 u = b - a;
	Vec3 v = c - a;
	return normalize(cross(u, v));
}

struct Vec4 {
	float x, y, z, w;

	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w) {
	}

	Vec4(const Vec3& v, float _w)
		: x(v.x), y(v.y), z(v.z), w(_w) {
	}
};

struct Mat3 {
	float m[9]; // column-major

	Mat3() {
		identity();
	}

	void identity() {
		memset(m, 0, sizeof(m));
		m[0] = m[4] = m[8] = 1.0f;
	}

	void setColumn(int c, const Vec3& v) {
		m[c * 3 + 0] = v.x;
		m[c * 3 + 1] = v.y;
		m[c * 3 + 2] = v.z;
	}

	Vec3 getColumn(int c) const {
		return {
			m[c * 3 + 0],
			m[c * 3 + 1],
			m[c * 3 + 2]
		};
	}

	Vec3 right()   const { return getColumn(0); }
	Vec3 up()      const { return getColumn(1); }
	Vec3 forward() const { return getColumn(2); }
};

inline Vec3 Mat3ToEuler_YXZ(const Mat3& m) {
	Vec3 e;

	// pitch (X)
	e.x = RAD2DEG * asinf(-m.m[7]); // -forward.y

	float cp = cosf(DEG2RAD * e.x);

	if (fabs(cp) > 1e-6f) {
		// yaw (Y)
		e.y = RAD2DEG * atan2f(m.m[6], m.m[8]); // forward.x, forward.z
		// roll (Z)
		e.z = RAD2DEG * atan2f(m.m[1], m.m[4]); // right.y, up.y
	}
	else {
		// Gimbal lock
		e.y = RAD2DEG * atan2f(-m.m[2], m.m[0]);
		e.z = 0;
	}

	return e;
}
// linear interpolation
inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
	return {
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t,
		a.z + (b.z - a.z) * t
	};
}
struct Mat4 {
	float m[16];
};

inline Mat4 Identity() {
	Mat4 r;
	for (int i = 0; i < 16; i++) r.m[i] = 0;
	r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1;
	return r;
}

inline Mat4 Translate(Vec3 t) {
	Mat4 r = Identity();
	r.m[12] = t.x;
	r.m[13] = t.y;
	r.m[14] = t.z;
	return r;
}

inline Mat4 RotateX(float deg) {
	float r = deg * M_PI / 180.f;
	Mat4 m = Identity();
	m.m[5] = cos(r);  m.m[6] = sin(r);
	m.m[9] = -sin(r); m.m[10] = cos(r);
	return m;
}

inline Mat4 RotateY(float deg) {
	float r = deg * M_PI / 180.f;
	Mat4 m = Identity();
	m.m[0] = cos(r);  m.m[2] = sin(r);
	m.m[8] = -sin(r); m.m[10] = cos(r);
	return m;
}

inline Mat4 RotateZ(float deg) {
	float r = deg * M_PI / 180.f;
	Mat4 m = Identity();
	m.m[0] = cos(r);  m.m[1] = -sin(r);
	m.m[4] = sin(r);  m.m[5] = cos(r);
	return m;
}
inline Mat4 Scale(Vec3 s) {
	Mat4 m = Identity();
	m.m[0] = s.x;
	m.m[5] = s.y;
	m.m[10] = s.z;
	return m;
}
inline Mat4 operator*(const Mat4& a, const Mat4& b) {
	Mat4 r;
	for (int i = 0; i < 16; i++) r.m[i] = 0;
	for (int c = 0; c < 4; c++)
		for (int r0 = 0; r0 < 4; r0++)
			for (int k = 0; k < 4; k++)
				r.m[c * 4 + r0] += a.m[k * 4 + r0] * b.m[c * 4 + k];
	return r;
}


inline Mat4 InverseTR(const Mat4& m) {
	Mat4 r = Identity();

	// transpose rotation
	r.m[0] = m.m[0]; r.m[1] = m.m[4]; r.m[2] = m.m[8];
	r.m[4] = m.m[1]; r.m[5] = m.m[5]; r.m[6] = m.m[9];
	r.m[8] = m.m[2]; r.m[9] = m.m[6]; r.m[10] = m.m[10];

	// inverse translation
	r.m[12] = -(r.m[0] * m.m[12] + r.m[4] * m.m[13] + r.m[8] * m.m[14]);
	r.m[13] = -(r.m[1] * m.m[12] + r.m[5] * m.m[13] + r.m[9] * m.m[14]);
	r.m[14] = -(r.m[2] * m.m[12] + r.m[6] * m.m[13] + r.m[10] * m.m[14]);

	return r;
}
inline Vec3 operator*(const Mat4& M, const Vec3& p) {
	Vec3 r;
	r.x = M.m[0] * p.x + M.m[4] * p.y + M.m[8] * p.z + M.m[12];
	r.y = M.m[1] * p.x + M.m[5] * p.y + M.m[9] * p.z + M.m[13];
	r.z = M.m[2] * p.x + M.m[6] * p.y + M.m[10] * p.z + M.m[14];
	return r;
}

inline Vec4 operator*(const Mat4& M, const Vec4& v) {
	Vec4 r;
	r.x = M.m[0] * v.x + M.m[4] * v.y + M.m[8] * v.z + M.m[12] * v.w;
	r.y = M.m[1] * v.x + M.m[5] * v.y + M.m[9] * v.z + M.m[13] * v.w;
	r.z = M.m[2] * v.x + M.m[6] * v.y + M.m[10] * v.z + M.m[14] * v.w;
	r.w = M.m[3] * v.x + M.m[7] * v.y + M.m[11] * v.z + M.m[15] * v.w;
	return r;
}
// =================== Transform ===================
class Transform {
public:
	Vec3 localPos = Vec3(0, 0, 0);
	Vec3 localRot = Vec3(0, 0, 0);
	Vec3 localScale = Vec3(1, 1, 1);

	Mat4 localMatrix;
	Mat4 worldMatrix;

	Transform* parent = nullptr;
	std::vector<Transform*> children;

	Mat4 computeLocal() {
		return
			Translate(localPos) *
			RotateZ(localRot.z) *
			RotateY(localRot.y) *
			RotateX(localRot.x) *
			Scale(localScale);
	}

	void updateWorld() {
		localMatrix = computeLocal();

		if (parent)
			worldMatrix = parent->worldMatrix * localMatrix;
		else
			worldMatrix = localMatrix;

		for (auto c : children)
			c->updateWorld();
	}

	void applyGL() {
		glMultMatrixf(worldMatrix.m);
	}

	Vec3 getWorldPosition() const {
		return Vec3(
			worldMatrix.m[12],
			worldMatrix.m[13],
			worldMatrix.m[14]
		);
	}
	Vec3 getWorldScale() const {
		if (!parent) return localScale;
		Vec3 ps = parent->getWorldScale();
		return {
			ps.x * localScale.x,
			ps.y * localScale.y,
			ps.z * localScale.z
		};
	}
	Vec3 getWorldRotation() const {
		if (!parent) return localRot;
		return parent->getWorldRotation() + localRot;
	}
	Vec3 right() const {
		return normalize(Vec3(
			worldMatrix.m[0],
			worldMatrix.m[1],
			worldMatrix.m[2]
		));
	}

	Vec3 up() const {
		return normalize(Vec3(
			worldMatrix.m[4],
			worldMatrix.m[5],
			worldMatrix.m[6]
		));
	}

	Vec3 forward() const {
		return normalize(Vec3(
			-worldMatrix.m[8],
			-worldMatrix.m[9],
			-worldMatrix.m[10]
		));
	}
	void setWorldBasisAndPosition(
		const Vec3& pos,
		const Vec3& right,
		const Vec3& up,
		const Vec3& forward
	) {
		setWorldPosition(pos);
		float pitch = asin(clamp(forward.y, -1.0f, 1.0f));
		float yaw = atan2(forward.x, -forward.z);

		localRot.x = pitch * RAD2DEG;
		localRot.y = yaw * RAD2DEG;
		localRot.z = 0.0f;
	}
	void setWorldRotationFromBasis(
		const Vec3& right,
		const Vec3& up,
		const Vec3& forward
	) {
		setWorldBasisAndPosition(getWorldPosition(), right, up, forward);
	}
	void setWorldPosition(const Vec3& P) {
		if (!parent) {
			localPos = P;
		}
		else {
			// world = parentWorld * local
			// => local = inverse(parentWorld) * world
			Mat4 invParent = InverseTR(parent->worldMatrix);
			Vec4 lp = invParent * Vec4(P, 1.0f);
			localPos = { lp.x, lp.y, lp.z };
		}
	}

	const Mat4& getWorldMatrix() const {
		return worldMatrix;
	}
	void lookAt(
		const Vec3& worldTarget,
		const Vec3& worldUp = Vec3(0, 1, 0)
	) {
		Vec3 pos = getWorldPosition();

		Vec3 forward = worldTarget - pos;
		if (length(forward) < 1e-6f)
			return;
		forward = normalize(forward);

		Vec3 up = worldUp;
		if (length(up) < 1e-6f)
			up = Vec3(0, 1, 0);

		Vec3 right = cross(forward, up);

		if (length(right) < 1e-6f) {
			up = Vec3(0, 0, 1);
			right = cross(forward, up);
		}

		right = normalize(right);
		up = cross(right, forward); 

		setWorldRotationFromBasis(right, up, forward);
	}


};


// =================== Collider ===================
enum class ColliderType {
	Sphere,
	AABB
};

class Collider {
public:
	ColliderType type;
	Transform* transform = nullptr;

	virtual void computeWorld() = 0;
	virtual bool intersects(const Collider* other) const = 0;

	virtual ~Collider() {}
};

class SphereCollider : public Collider {
public:
	float radius;
	Vec3 centerWorld;
	float worldRadius;

	SphereCollider(float r) : radius(r) {
		type = ColliderType::Sphere;
	}

	void computeWorld() override {
		centerWorld = transform->getWorldPosition();

		Vec3 s = transform->getWorldScale();
		float maxScale = MAX(s.x, MAX(s.y, s.z));
		worldRadius = radius * maxScale;
	}


	bool intersects(const Collider* other) const override {
		if (other->type != ColliderType::Sphere) return false;

		const SphereCollider* s = (const SphereCollider*)other;
		float dist = length(centerWorld - s->centerWorld);
		//return dist <= (radius + s->radius);
		return dist <= (worldRadius + s->worldRadius);
	}
};

class AABBCollider : public Collider {
public:
	Vec3 minLocal, maxLocal;
	Vec3 minWorld, maxWorld;

	AABBCollider(Vec3 minL, Vec3 maxL)
		: minLocal(minL), maxLocal(maxL) {
		type = ColliderType::AABB;
	}

	void computeWorld() override {
		Mat4 M = transform->getWorldMatrix();

		Vec3 corners[8] = {
			{minLocal.x, minLocal.y, minLocal.z},
			{maxLocal.x, minLocal.y, minLocal.z},
			{minLocal.x, maxLocal.y, minLocal.z},
			{maxLocal.x, maxLocal.y, minLocal.z},
			{minLocal.x, minLocal.y, maxLocal.z},
			{maxLocal.x, minLocal.y, maxLocal.z},
			{minLocal.x, maxLocal.y, maxLocal.z},
			{maxLocal.x, maxLocal.y, maxLocal.z}
		};

		minWorld = maxWorld = M * corners[0];

		for (int i = 1; i < 8; i++) {
			Vec3 p = M * corners[i];
			minWorld = min(minWorld, p);
			maxWorld = max(maxWorld, p);
		}
	}


	bool intersects(const Collider* o) const override {
		if (o->type != ColliderType::AABB) return false;
		const AABBCollider* b = (const AABBCollider*)o;

		return (minWorld.x <= b->maxWorld.x && maxWorld.x >= b->minWorld.x) &&
			(minWorld.y <= b->maxWorld.y && maxWorld.y >= b->minWorld.y) &&
			(minWorld.z <= b->maxWorld.z && maxWorld.z >= b->minWorld.z);
	}
};

// =================== Mesh ===================
struct Vertex {
	Vec3 pos;
	Vec3 normal;
	Vec2 tex;
};

struct Triangle {
	unsigned int i0, i1, i2;
};

struct Quad {
	unsigned int i0, i1, i2, i3;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;
	std::vector<Quad> quads;

	void clear() {
		vertices.clear();
		triangles.clear();
		quads.clear();
	}

	void draw() const {
		glBegin(GL_TRIANGLES);
		for (auto& t : triangles) {
			drawVertex(t.i0);
			drawVertex(t.i1);
			drawVertex(t.i2);
		}
		glEnd();

		glBegin(GL_QUADS);
		for (auto& q : quads) {
			drawVertex(q.i0);
			drawVertex(q.i1);
			drawVertex(q.i2);
			drawVertex(q.i3);
		}
		glEnd();
	}

private:
	inline void drawVertex(unsigned int i) const {
		const Vertex& v = vertices[i];
		glNormal3f(v.normal.x, v.normal.y, v.normal.z);
		glTexCoord2f(v.tex.x, v.tex.y);
		glVertex3f(v.pos.x, v.pos.y, v.pos.z);
	}
};
inline void computeNormals(Mesh& mesh) {
	for (auto& v : mesh.vertices)
		v.normal ={ 0, 0, 0 };

	auto addNormal = [&](unsigned int a, unsigned int b, unsigned int c) {
		Vec3& A = mesh.vertices[a].pos;
		Vec3& B = mesh.vertices[b].pos;
		Vec3& C = mesh.vertices[c].pos;

		Vec3 n = normalize(cross(B - A, C - A));

		mesh.vertices[a].normal += n;
		mesh.vertices[b].normal += n;
		mesh.vertices[c].normal += n;
	};

	for (auto& t : mesh.triangles)
		addNormal(t.i0, t.i1, t.i2);

	for (auto& q : mesh.quads) {
		addNormal(q.i0, q.i1, q.i2);
		addNormal(q.i0, q.i2, q.i3);
	}

	for (auto& v : mesh.vertices)
		v.normal = normalize(v.normal);
}

inline void computeAABB(const Mesh& mesh, Vec3& minv, Vec3& maxv) {
	if (mesh.vertices.empty()) {
		minv = maxv ={ 0,0,0 };
		return;
	}

	minv = maxv = mesh.vertices[0].pos;

	for (const auto& v : mesh.vertices) {
		minv.x = MIN(minv.x, v.pos.x);
		minv.y = MIN(minv.y, v.pos.y);
		minv.z = MIN(minv.z, v.pos.z);

		maxv.x = MAX(maxv.x, v.pos.x);
		maxv.y = MAX(maxv.y, v.pos.y);
		maxv.z = MAX(maxv.z, v.pos.z);
	}
}
inline void centerMesh(Mesh& mesh) {
	Vec3 minv, maxv;
	computeAABB(mesh, minv, maxv);

	Vec3 center{
		0.5f * (minv.x + maxv.x),
		0.5f * (minv.y + maxv.y),
		0.5f * (minv.z + maxv.z)
	};

	for (auto& v : mesh.vertices) {
		v.pos.x -= center.x;
		v.pos.y -= center.y;
		v.pos.z -= center.z;
	}
}
inline void scaleToUnitSize(Mesh& mesh) {
	Vec3 minv, maxv;
	computeAABB(mesh, minv, maxv);

	Vec3 size{
		maxv.x - minv.x,
		maxv.y - minv.y,
		maxv.z - minv.z
	};

	float maxExtent = MAX(MAX(size.x, size.y), size.z );
	if (maxExtent <= 0.0f) return;

	float scale = 1.0f / maxExtent;

	for (auto& v : mesh.vertices) {
		v.pos.x *= scale;
		v.pos.y *= scale;
		v.pos.z *= scale;
	}
}
inline void normalizeMesh(Mesh& mesh) {
	centerMesh(mesh);
	scaleToUnitSize(mesh);
}
// =================== MeshRenderer ===================
enum class RenderMode {
	Solid,
	Wireframe,
	Textured
};
inline void setSolidMaterial(const Vec3& c) {
	GLfloat diffuse[]  ={ c.x, c.y, c.z, 1.0f };
	GLfloat ambient[]  ={ 0.2f * c.x, 0.2f * c.y, 0.2f * c.z, 1.0f };
	GLfloat specular[] ={ 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat shininess  = 16.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

class MeshRenderer {
public:
	Mesh* mesh = nullptr;
	GLuint texture = 0;

	RenderMode mode = RenderMode::Textured;

	Vec3 color ={ 0.8f, 0.8f, 0.8f };

	void draw() const {
		if (!mesh) return;

		// --- Save states ---
		glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_LIGHTING_BIT);

		switch (mode) {

		case RenderMode::Wireframe:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glColor3f(1.0f, 1.0f, 1.0f);
			break;

		case RenderMode::Textured:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			if (texture != 0) {
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture);
				glEnable(GL_LIGHTING);
			} else {
				// fallback -> solid
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
				setSolidMaterial(color);
			}
			break;

		case RenderMode::Solid:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
			setSolidMaterial(color);
			break;
		}

		mesh->draw();

		// --- Restore states ---
		glPopAttrib();
	}

};


// =================== VirtualObject ===================
class VirtualObject {
public:
	VirtualObject(std::string name = "") :name(name) {}
	std::string name = "";

	Transform transform;
	MeshRenderer* meshRenderer = nullptr;
	std::vector<VirtualObject*> children;
	Collider* collider = nullptr;

	void addChild(VirtualObject* obj) {
		obj->transform.parent = &transform;
		transform.children.push_back(&(obj->transform));
		children.push_back(obj);
	}

	virtual void update(float dt) {
		if (collider) collider->computeWorld();

		for (auto c : children) c->update(dt);
	}
};

// =================== Camera ===================
class CameraComponent {
public:
	float fov = 60;
	float aspect = 4.f / 3.f;
	float nearZ = 0.1f;
	float farZ = 100;

	Transform* transform = nullptr;

	void applyView() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(fov, aspect, nearZ, farZ);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Vec3 pos = transform->getWorldPosition();
		Vec3 forward = transform->forward();
		Vec3 up = transform->up();
		Vec3 target = pos + forward;

		//printf("camera %f %f %f %f %f %f\n", pos.x, pos.y, pos.z, target.x, target.y, target.z);
		gluLookAt(
			pos.x, pos.y, pos.z,
			target.x, target.y, target.z,
			up.x, up.y, up.z
		);
	}
};
class ICameraController {
public:
	virtual ~ICameraController() = default;

	virtual void setTransform(Transform* t) = 0;
	virtual void update(float dt) = 0;

	// Input hooks
	virtual void onMouseButton(int btn, int state, int x, int y) {}
	virtual void onMouseMove(int x, int y) {}
	virtual void onKey(int key, bool down) {}
};
class SimpleCameraController : public ICameraController {
public:
	Transform* transform = nullptr;

	void setTransform(Transform* t) override {
		transform = t;
	}

	void update(float) override {
		// nothing per-frame
	}
};
class OrbitCameraController : public ICameraController {
public:
	Transform* transform = nullptr;

	Vec3 target = { 0,0,0 };
	float distance = 10.0f;
	float yaw = -90.0f;
	float pitch = 20.0f;

	float rotSpeed = 0.3f;
	float panSpeed = 0.01f;
	float zoomSpeed = 0.5f;

	bool rotating = false;
	bool panning = false;
	int lastX = 0, lastY = 0;

	void setTransform(Transform* t) override {
		transform = t;
	}

	void update(float) override {
		updateTransform();
	}

	void lookAt(const Vec3& camPos,	const Vec3& tgt)  {
		target = tgt;

		Vec3 offset = camPos - target;
		distance = length(offset);

		// Pitch
		float pitchRad = asinf(offset.y / distance);
		pitch = RAD2DEG*pitchRad;

		// Yaw
		float yawRad = atan2f(offset.x, offset.z);
		yaw = RAD2DEG*yawRad;

		updateTransform();
	}

	void updateTransform() {
		float yawRad = DEG2RAD*yaw;
		float pitchRad = DEG2RAD*pitch;

		Vec3 offset;
		offset.x = distance * cosf(pitchRad) * sinf(yawRad);
		offset.y = distance * sinf(pitchRad);
		offset.z = distance * cosf(pitchRad) * cosf(yawRad);

		transform->localPos = target + offset;
		transform->localRot = { -pitch, yaw + 180.0f, 0 };
	}

	// ===== Input =====
	void onMouseButton(int button, int state, int x, int y) override {
		if (state == GLUT_DOWN) {
			lastX = x; lastY = y;
			if (button == GLUT_RIGHT_BUTTON) rotating = true;
			if (button == GLUT_MIDDLE_BUTTON) panning = true;
		}
		else {
			rotating = panning = false;
		}

		if (button == 3) distance -= zoomSpeed;
		if (button == 4) distance += zoomSpeed;

		distance = MAX(1.f, distance);
		updateTransform();
	}

	void onMouseMove(int x, int y) override {
		int dx = x - lastX;
		int dy = y - lastY;

		if (rotating) {
			yaw += dx * rotSpeed;
			pitch += dy * rotSpeed;
			pitch = clamp(pitch, -89.f, 89.f);
		}

		if (panning) {
			Vec3 f = transform->forward();
			Vec3 r = normalize(cross(f, { 0,1,0 }));
			Vec3 u = cross(r, f);

			target += (-1* r * dx + u * dy) * panSpeed * distance;
		}

		lastX = x; lastY = y;
		updateTransform();
	}
};

class CameraObject : public VirtualObject {
public:
	CameraComponent camera;
	ICameraController* controller = nullptr;

	CameraObject(std::string name="") :VirtualObject(name) {
		camera.transform = &transform; 
	}

	void setController(ICameraController* c) {
		controller = c;
		controller->setTransform(&transform);
	}

	void update(float dt) override {
		if (controller)
			controller->update(dt);

		transform.updateWorld();
	}
};



inline Mesh* createUnitSphere(int slices = 16, int stacks = 16) {
	Mesh* m = new Mesh();

	const float radius = 0.5f;

	// ===== Vertices =====
	for (int i = 0; i <= stacks; i++) {
		float v = (float)i / stacks;
		float phi = v * M_PI; // 0 -> PI

		for (int j = 0; j <= slices; j++) {
			float u = (float)j / slices;
			float theta = u * 2.0f * M_PI; // 0 -> 2PI

			float x = sin(phi) * cos(theta);
			float y = cos(phi);
			float z = sin(phi) * sin(theta);

			Vertex vert;
			vert.pos    ={ radius * x, radius * y, radius * z };
			vert.normal ={ x, y, z };      // unit sphere -> normal = position
			vert.tex    ={ u, 1.0f - v };  // OBJ / OpenGL UV convention

			m->vertices.push_back(vert);
		}
	}

	// ===== Faces (Quads) =====
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			int row1 = i * (slices + 1);
			int row2 = (i + 1) * (slices + 1);

			unsigned int A = row1 + j;
			unsigned int B = row1 + j + 1;
			unsigned int C = row2 + j + 1;
			unsigned int D = row2 + j;

			m->quads.push_back({ A, B, C, D });
		}
	}

	return m;
}


inline Mesh* createGridPlane(int halfSize = 15, float cellSize = 1.0f) {
	Mesh* m = new Mesh();

	int size = halfSize * 2;
	int row  = size + 1;

	// ===== Vertices =====
	for (int z = 0; z <= size; z++) {
		for (int x = 0; x <= size; x++) {

			float px = (x - halfSize) * cellSize;
			float pz = (z - halfSize) * cellSize;

			Vertex v;
			v.pos    ={ px, 0.0f, pz };
			v.normal ={ 0.0f, 1.0f, 0.0f };

			// UV scale theo world unit
			v.tex.x = (float)x / size;
			v.tex.y = (float)z / size;

			m->vertices.push_back(v);
		}
	}

	// ===== Quads =====
	for (int z = 0; z < size; z++) {
		for (int x = 0; x < size; x++) {

			unsigned int A =  z      * row + x;
			unsigned int B =  A + 1;
			unsigned int C =  A + row + 1;
			unsigned int D =  A + row;

			m->quads.push_back({ A, B, C, D });
		}
	}

	return m;
}


inline Mesh* createUnitCube() {
	Mesh* m = new Mesh();

	const float h = 0.5f;

	// ===== Helper lambda =====
	auto addFace = [&](Vec3 n, Vec3 a, Vec3 b, Vec3 c, Vec3 d) {
		unsigned int start = (unsigned int)m->vertices.size();

		m->vertices.push_back({ a, n,{ 0,0 } });
		m->vertices.push_back({ b, n,{ 1,0 } });
		m->vertices.push_back({ c, n,{ 1,1 } });
		m->vertices.push_back({ d, n,{ 0,1 } });

		m->quads.push_back({ start, start+1, start+2, start+3 });
	};

	// ===== Faces =====

	// Front (+Z)
	addFace(
	{ 0, 0, 1 },
	{ -h, -h,  h },
	{ h, -h,  h },
	{ h,  h,  h },
	{ -h,  h,  h }
	);

	// Back (-Z)
	addFace(
	{ 0, 0,-1 },
	{ h, -h, -h },
	{ -h, -h, -h },
	{ -h,  h, -h },
	{ h,  h, -h }
	);

	// Right (+X)
	addFace(
	{ 1, 0, 0 },
	{ h, -h,  h },
	{ h, -h, -h },
	{ h,  h, -h },
	{ h,  h,  h }
	);

	// Left (-X)
	addFace(
	{ -1, 0, 0 },
	{ -h, -h, -h },
	{ -h, -h,  h },
	{ -h,  h,  h },
	{ -h,  h, -h }
	);

	// Top (+Y)
	addFace(
	{ 0, 1, 0 },
	{ -h,  h,  h },
	{ h,  h,  h },
	{ h,  h, -h },
	{ -h,  h, -h }
	);

	// Bottom (-Y)
	addFace(
	{ 0,-1, 0 },
	{ -h, -h, -h },
	{ h, -h, -h },
	{ h, -h,  h },
	{ -h, -h,  h }
	);

	return m;
}


// =================== Render ===================
inline void drawAxis(float s = 2.0f, float w = 4.0f) {
	glLineWidth(w);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(s, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, s, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, s);
	glEnd();
}

inline void renderObject(VirtualObject* obj, bool bAxis = false) {
	glPushMatrix();
	//glLoadMatrixf(obj->transform.worldMatrix.m);
	glMultMatrixf(obj->transform.worldMatrix.m);

	if (obj->meshRenderer) {
		if (bAxis) drawAxis();

		glColor3f(0.9f, 0.9f, 0.9f);

		obj->meshRenderer->draw();
	}

	glPopMatrix();

	for (auto c : obj->children)
		renderObject(c, bAxis);
}
// =================== Actor ===================
class Actor {
public:
	virtual void reset() {}
	virtual void update(float dt) {}
};
}

#endif