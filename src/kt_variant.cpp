#include "kt_variant.h"

// must match the value order of godot_variant_type
static void (* TO_KT_VARIANT_FROM[27 /* Variant::Type count */])(wire::Value&, const Variant&);

static Variant (* TO_GODOT_VARIANT_FROM[27 /* KVariant::TypeCase count */])(const wire::Value&);

static Variant::Type WIRE_TYPE_CASE_TO_VARIANT_TYPE[15];
static String VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[28];


KtVariant::KtVariant(wire::Value value) : value(value) {}

void to_kvariant_fromNIL(wire::Value& des, const Variant& src) {
    des.set_nil_value(0);
}

void to_kvariant_fromINT(wire::Value& des, const Variant& src) {
    des.set_long_value(src);
}

void to_kvariant_fromREAL(wire::Value& des, const Variant& src) {
    des.set_real_value(src);
}

void to_kvariant_fromSTRING(wire::Value& des, const Variant& src) {
    String str = src;
    des.set_string_value(str.utf8().get_data());
}

void to_kvariant_fromBOOL(wire::Value& des, const Variant& src) {
    des.set_bool_value(src);
}

inline wire::Vector2* to_wire_vector2(const Vector2& from) {
    auto vec2 = wire::Vector2::default_instance().New();
    vec2->set_x(from.x);
    vec2->set_y(from.y);
    return vec2;
}

void to_kvariant_fromVECTOR2(wire::Value& des, const Variant& src) {
    des.set_allocated_vector2_value(to_wire_vector2(src));
}

void to_kvariant_fromRECT2(wire::Value& des, const Variant& src) {
    auto wire_rect2 = wire::Rect2::default_instance().New();
    Rect2 src_rect2 = src;
    wire_rect2->set_allocated_position(to_wire_vector2(src_rect2.position));
    wire_rect2->set_allocated_size(to_wire_vector2(src_rect2.size));
    des.set_allocated_rect2_value(wire_rect2);
}

inline wire::Vector3* to_wire_vector3(const Vector3& from) {
    auto vec3 = wire::Vector3::default_instance().New();
    vec3->set_x(from.x);
    vec3->set_y(from.y);
    vec3->set_z(from.z);
    return vec3;
}

void to_kvariant_fromVECTOR3(wire::Value& des, const Variant& src) {
    des.set_allocated_vector3_value(to_wire_vector3(src));
}

void to_kvariant_fromTRANSFORM2D(wire::Value& des, const Variant& src) {
    auto transform_2d = wire::Transform2D::default_instance().New();
    Transform2D src_transform_2d = src;
    transform_2d->set_allocated_x(to_wire_vector2(src_transform_2d.get_axis(0)));
    transform_2d->set_allocated_y(to_wire_vector2(src_transform_2d.get_axis(1)));
    transform_2d->set_allocated_origin(to_wire_vector2(src_transform_2d.get_origin()));
    des.set_allocated_transform2d_value(transform_2d);
}

void to_kvariant_fromPLANE(wire::Value& des, const Variant& src) {
    auto plane = wire::Plane::default_instance().New();
    Plane src_plane = src;
    plane->set_allocated_normal(to_wire_vector3(src_plane.normal));
    plane->set_d(src_plane.d);
    des.set_allocated_plane_value(plane);
}

void to_kvariant_fromQUAT(wire::Value& des, const Variant& src) {
    auto quat = wire::Quat::default_instance().New();
    Quat src_quat = src;
    quat->set_x(src_quat.x);
    quat->set_y(src_quat.y);
    quat->set_z(src_quat.z);
    quat->set_w(src_quat.w);
    des.set_allocated_quat_value(quat);
}

void to_kvariant_fromAABB(wire::Value& des, const Variant& src) {
    auto aabb = wire::AABB::default_instance().New();
    AABB src_aabb = src;
    aabb->set_allocated_position(to_wire_vector3(src_aabb.position));
    aabb->set_allocated_size(to_wire_vector3(src_aabb.size));
    des.set_allocated_aabb_value(aabb);
}

inline wire::Basis* to_wire_basis(const Basis& data) {
    auto basis = wire::Basis::default_instance().New();
    basis->set_allocated_x(to_wire_vector3(data.get_axis(0)));
    basis->set_allocated_y(to_wire_vector3(data.get_axis(1)));
    basis->set_allocated_z(to_wire_vector3(data.get_axis(2)));
    return basis;
}

void to_kvariant_fromBASIS(wire::Value& des, const Variant& src) {
    des.set_allocated_basis_value(to_wire_basis(src));
}

void to_kvariant_fromTRANSFORM(wire::Value& des, const Variant& src) {
    auto transform = wire::Transform::default_instance().New();
    Transform src_transform = src;
    transform->set_allocated_basis(to_wire_basis(src_transform.basis));
    transform->set_allocated_origin(to_wire_vector3(src_transform.origin));
    des.set_allocated_transform_value(transform);
}

KtVariant::KtVariant(const Variant& variant) {
    Variant::Type type = variant.get_type();
    TO_KT_VARIANT_FROM[type](value, variant);
}

const wire::Value& KtVariant::get_value() const {
    return value;
}

Variant from_kvariant_tokNilValue(const wire::Value& src) {
    return Variant();
}

Variant from_kvariant_tokLongValue(const wire::Value& src) {
    return Variant(src.long_value());
}

Variant from_kvariant_tokRealValue(const wire::Value& src) {
    return Variant(src.real_value());
}

Variant from_kvariant_tokStringValue(const wire::Value& src) {
    return Variant(String(src.string_value().c_str()));
}

Variant from_kvariant_tokBoolValue(const wire::Value& src) {
    return Variant(src.bool_value());
}

inline Vector2 to_godot_vector2(const wire::Vector2& data) {
    return {data.x(), data.y()};
}

Variant from_kvariant_tokVector2Value(const wire::Value& src) {
    return Variant(to_godot_vector2(src.vector2_value()));
}

Variant from_kvariant_tokRect2Value(const wire::Value& src) {
    return Variant(
            Rect2(to_godot_vector2(src.rect2_value().position()), to_godot_vector2(src.rect2_value().size()))
    );
}

inline Vector3 to_godot_vector3(const wire::Vector3& data) {
    return {data.x(), data.y(), data.z()};
}

Variant from_kvariant_tokVector3Value(const wire::Value& src) {
    return Variant(to_godot_vector3(src.vector3_value()));
}

Variant from_kvariant_tokTransform2DValue(const wire::Value& src) {
    Transform2D transform2d;
    transform2d.set_axis(0, to_godot_vector2(src.transform2d_value().x()));
    transform2d.set_axis(1, to_godot_vector2(src.transform2d_value().y()));
    transform2d.set_origin(to_godot_vector2(src.transform2d_value().origin()));
    return Variant(transform2d);
}

Variant from_kvariant_tokPlaneValue(const wire::Value& src) {
    return Variant(
            Plane(to_godot_vector3(src.plane_value().normal()), src.plane_value().d())
    );
}

Variant from_kvariant_tokQuatValue(const wire::Value& src) {
    return Variant(
            Quat(src.quat_value().x(), src.quat_value().y(), src.quat_value().z(), src.quat_value().w())
    );
}

Variant from_kvariant_tokAabbValue(const wire::Value& src) {
    return Variant(
            AABB(to_godot_vector3(src.aabb_value().position()), to_godot_vector3(src.aabb_value().size()))
    );
}

inline Basis to_godot_basis(const wire::Basis& data) {
    return {to_godot_vector3(data.x()), to_godot_vector3(data.y()), to_godot_vector3(data.z())};
}

Variant from_kvariant_tokBasisValue(const wire::Value& src) {
    return Variant(to_godot_basis(src.basis_value()));
}

Variant from_kvariant_tokTransformValue(const wire::Value& src) {
    return Variant(
            Transform(to_godot_basis(src.transform_value().basis()), to_godot_vector3(src.transform_value().origin()))
    );
}

Variant KtVariant::to_godot_variant() const {
    return TO_GODOT_VARIANT_FROM[value.type_case()](value);
}

void KtVariant::initMethodArray() {
    TO_KT_VARIANT_FROM[Variant::NIL] = to_kvariant_fromNIL;
    TO_KT_VARIANT_FROM[Variant::BOOL] = to_kvariant_fromBOOL;
    TO_KT_VARIANT_FROM[Variant::INT] = to_kvariant_fromINT;
    TO_KT_VARIANT_FROM[Variant::REAL] = to_kvariant_fromREAL;
    TO_KT_VARIANT_FROM[Variant::STRING] = to_kvariant_fromSTRING;
    TO_KT_VARIANT_FROM[Variant::VECTOR2] = to_kvariant_fromVECTOR2;
    TO_KT_VARIANT_FROM[Variant::RECT2] = to_kvariant_fromRECT2;
    TO_KT_VARIANT_FROM[Variant::VECTOR3] = to_kvariant_fromVECTOR3;
    TO_KT_VARIANT_FROM[Variant::TRANSFORM2D] = to_kvariant_fromTRANSFORM2D;
    TO_KT_VARIANT_FROM[Variant::PLANE] = to_kvariant_fromPLANE;
    TO_KT_VARIANT_FROM[Variant::QUAT] = to_kvariant_fromQUAT;
    TO_KT_VARIANT_FROM[Variant::AABB] = to_kvariant_fromAABB;
    TO_KT_VARIANT_FROM[Variant::BASIS] = to_kvariant_fromBASIS;
    TO_KT_VARIANT_FROM[Variant::TRANSFORM] = to_kvariant_fromTRANSFORM;

    TO_GODOT_VARIANT_FROM[wire::Value::kNilValue - 1] = from_kvariant_tokNilValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kBoolValue - 1] = from_kvariant_tokBoolValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kLongValue - 1] = from_kvariant_tokLongValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kRealValue - 1] = from_kvariant_tokRealValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kStringValue - 1] = from_kvariant_tokStringValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kVector2Value - 1] = from_kvariant_tokVector2Value;
    TO_GODOT_VARIANT_FROM[wire::Value::kRect2Value - 1] = from_kvariant_tokRect2Value;
    TO_GODOT_VARIANT_FROM[wire::Value::kVector3Value - 1] = from_kvariant_tokVector3Value;
    TO_GODOT_VARIANT_FROM[wire::Value::kTransform2DValue - 1] = from_kvariant_tokTransform2DValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kPlaneValue - 1] = from_kvariant_tokPlaneValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kQuatValue - 1] = from_kvariant_tokQuatValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kAabbValue - 1] = from_kvariant_tokAabbValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kBasisValue - 1] = from_kvariant_tokBasisValue;
    TO_GODOT_VARIANT_FROM[wire::Value::kTransformValue - 1] = from_kvariant_tokTransformValue;

    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kNilValue] = Variant::Type::NIL;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kBoolValue] = Variant::Type::BOOL;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kLongValue] = Variant::Type::INT;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kRealValue] = Variant::Type::REAL;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kStringValue] = Variant::Type::STRING;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kVector2Value] = Variant::Type::VECTOR2;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kRect2Value] = Variant::Type::RECT2;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kVector3Value] = Variant::Type::VECTOR3;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kTransform2DValue] = Variant::Type::TRANSFORM2D;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kPlaneValue] = Variant::Type::PLANE;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kQuatValue] = Variant::Type::QUAT;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kAabbValue] = Variant::Type::AABB;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kBasisValue] = Variant::Type::BASIS;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::kTransformValue] = Variant::Type::TRANSFORM;
    WIRE_TYPE_CASE_TO_VARIANT_TYPE[wire::Value::TYPE_NOT_SET] = Variant::Type::VARIANT_MAX;

    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::NIL] = "V";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::BOOL] = "Z";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::INT] = "J";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::REAL] = "D";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::STRING] = "Ljava/lang/String;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::VECTOR2] = "Lgodot/core/Vector2;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::RECT2] = "Lgodot/core/Rect2;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::VECTOR3] = "Lgodot/core/Vector3;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::TRANSFORM2D] = "Lgodot/core/Transform2D;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::PLANE] = "Lgodot/core/Plane;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::QUAT] = "Lgodot/core/Quat;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::AABB] = "Lgodot/core/AABB;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::BASIS] = "Lgodot/core/Basis;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::TRANSFORM] = "Lgodot/core/Transform;";
    VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[Variant::Type::VARIANT_MAX] = "V";
}

Variant::Type KtVariant::fromWireTypeToVariantType(wire::Value::TypeCase typeCase) {
    return WIRE_TYPE_CASE_TO_VARIANT_TYPE[typeCase];
}

String KtVariant::fromVariantTypeToJvmString(Variant::Type type) {
    return VARIANT_TYPE_TO_JVM_SIGNATURE_STRING[type];
}