// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: insmessage.proto

#include "insmessage.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace raven {
namespace fcu {
namespace swicd {
class RavenFCU_SWICD_INSmessageDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<RavenFCU_SWICD_INSmessage> _instance;
} _RavenFCU_SWICD_INSmessage_default_instance_;
}  // namespace swicd
}  // namespace fcu
}  // namespace raven
static void InitDefaultsscc_info_RavenFCU_SWICD_INSmessage_insmessage_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::raven::fcu::swicd::_RavenFCU_SWICD_INSmessage_default_instance_;
    new (ptr) ::raven::fcu::swicd::RavenFCU_SWICD_INSmessage();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::raven::fcu::swicd::RavenFCU_SWICD_INSmessage::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_RavenFCU_SWICD_INSmessage_insmessage_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_RavenFCU_SWICD_INSmessage_insmessage_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_insmessage_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_insmessage_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_insmessage_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_insmessage_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, timestamp_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, latitude_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, longitude_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, altitude_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, roll_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, pitch_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, yaw_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, accel_x_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, accel_y_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, accel_z_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, barometer_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, course_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, speed_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, velocityn_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, velocitye_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, velocityd_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, numsats_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, declination_),
  PROTOBUF_FIELD_OFFSET(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage, gpsfix_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::raven::fcu::swicd::RavenFCU_SWICD_INSmessage)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::raven::fcu::swicd::_RavenFCU_SWICD_INSmessage_default_instance_),
};

const char descriptor_table_protodef_insmessage_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\020insmessage.proto\022\017raven.fcu.swicd\"\215\004\n\031"
  "RavenFCU_SWICD_INSmessage\022\021\n\ttimestamp\030\001"
  " \001(\001\022\020\n\010latitude\030\002 \001(\001\022\021\n\tlongitude\030\003 \001("
  "\001\022\020\n\010altitude\030\004 \001(\001\022\014\n\004roll\030\005 \001(\002\022\r\n\005pit"
  "ch\030\006 \001(\002\022\013\n\003yaw\030\007 \001(\002\022\017\n\007accel_x\030\010 \001(\002\022\017"
  "\n\007accel_y\030\t \001(\002\022\017\n\007accel_z\030\n \001(\002\022\021\n\tbaro"
  "meter\030\013 \001(\002\022\016\n\006course\030\014 \001(\002\022\r\n\005speed\030\r \001"
  "(\002\022\021\n\tvelocityN\030\016 \001(\002\022\021\n\tvelocityE\030\017 \001(\002"
  "\022\021\n\tvelocityD\030\020 \001(\002\022\017\n\007numSats\030\021 \001(\005\022\023\n\013"
  "declination\030\022 \001(\002\022B\n\006gpsFix\030\023 \001(\01622.rave"
  "n.fcu.swicd.RavenFCU_SWICD_INSmessage.Fi"
  "xType\"t\n\007FixType\022\016\n\nGPS_NO_FIX\020\000\022\026\n\022GPS_"
  "DEAD_RECKONING\020\001\022\n\n\006GPS_2D\020\002\022\n\n\006GPS_3D\020\003"
  "\022\026\n\022GPS_GNSS_DEAD_RECK\020\004\022\021\n\rGPS_TIME_ONL"
  "Y\020\005b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_insmessage_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_insmessage_2eproto_sccs[1] = {
  &scc_info_RavenFCU_SWICD_INSmessage_insmessage_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_insmessage_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_insmessage_2eproto = {
  false, false, descriptor_table_protodef_insmessage_2eproto, "insmessage.proto", 571,
  &descriptor_table_insmessage_2eproto_once, descriptor_table_insmessage_2eproto_sccs, descriptor_table_insmessage_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_insmessage_2eproto::offsets,
  file_level_metadata_insmessage_2eproto, 1, file_level_enum_descriptors_insmessage_2eproto, file_level_service_descriptors_insmessage_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_insmessage_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_insmessage_2eproto)), true);
namespace raven {
namespace fcu {
namespace swicd {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* RavenFCU_SWICD_INSmessage_FixType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_insmessage_2eproto);
  return file_level_enum_descriptors_insmessage_2eproto[0];
}
bool RavenFCU_SWICD_INSmessage_FixType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::GPS_NO_FIX;
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::GPS_DEAD_RECKONING;
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::GPS_2D;
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::GPS_3D;
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::GPS_GNSS_DEAD_RECK;
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::GPS_TIME_ONLY;
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::FixType_MIN;
constexpr RavenFCU_SWICD_INSmessage_FixType RavenFCU_SWICD_INSmessage::FixType_MAX;
constexpr int RavenFCU_SWICD_INSmessage::FixType_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

void RavenFCU_SWICD_INSmessage::InitAsDefaultInstance() {
}
class RavenFCU_SWICD_INSmessage::_Internal {
 public:
};

RavenFCU_SWICD_INSmessage::RavenFCU_SWICD_INSmessage(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
}
RavenFCU_SWICD_INSmessage::RavenFCU_SWICD_INSmessage(const RavenFCU_SWICD_INSmessage& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&timestamp_, &from.timestamp_,
    static_cast<size_t>(reinterpret_cast<char*>(&gpsfix_) -
    reinterpret_cast<char*>(&timestamp_)) + sizeof(gpsfix_));
  // @@protoc_insertion_point(copy_constructor:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
}

void RavenFCU_SWICD_INSmessage::SharedCtor() {
  ::memset(&timestamp_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&gpsfix_) -
      reinterpret_cast<char*>(&timestamp_)) + sizeof(gpsfix_));
}

RavenFCU_SWICD_INSmessage::~RavenFCU_SWICD_INSmessage() {
  // @@protoc_insertion_point(destructor:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void RavenFCU_SWICD_INSmessage::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void RavenFCU_SWICD_INSmessage::ArenaDtor(void* object) {
  RavenFCU_SWICD_INSmessage* _this = reinterpret_cast< RavenFCU_SWICD_INSmessage* >(object);
  (void)_this;
}
void RavenFCU_SWICD_INSmessage::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void RavenFCU_SWICD_INSmessage::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const RavenFCU_SWICD_INSmessage& RavenFCU_SWICD_INSmessage::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_RavenFCU_SWICD_INSmessage_insmessage_2eproto.base);
  return *internal_default_instance();
}


void RavenFCU_SWICD_INSmessage::Clear() {
// @@protoc_insertion_point(message_clear_start:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&timestamp_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&gpsfix_) -
      reinterpret_cast<char*>(&timestamp_)) + sizeof(gpsfix_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* RavenFCU_SWICD_INSmessage::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // double timestamp = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 9)) {
          timestamp_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // double latitude = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 17)) {
          latitude_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // double longitude = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 25)) {
          longitude_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // double altitude = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 33)) {
          altitude_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else goto handle_unusual;
        continue;
      // float roll = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 45)) {
          roll_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float pitch = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 53)) {
          pitch_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float yaw = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 61)) {
          yaw_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float accel_x = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 69)) {
          accel_x_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float accel_y = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 77)) {
          accel_y_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float accel_z = 10;
      case 10:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 85)) {
          accel_z_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float barometer = 11;
      case 11:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 93)) {
          barometer_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float course = 12;
      case 12:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 101)) {
          course_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float speed = 13;
      case 13:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 109)) {
          speed_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float velocityN = 14;
      case 14:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 117)) {
          velocityn_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float velocityE = 15;
      case 15:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 125)) {
          velocitye_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float velocityD = 16;
      case 16:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 133)) {
          velocityd_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // int32 numSats = 17;
      case 17:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 136)) {
          numsats_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // float declination = 18;
      case 18:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 149)) {
          declination_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // .raven.fcu.swicd.RavenFCU_SWICD_INSmessage.FixType gpsFix = 19;
      case 19:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 152)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_gpsfix(static_cast<::raven::fcu::swicd::RavenFCU_SWICD_INSmessage_FixType>(val));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* RavenFCU_SWICD_INSmessage::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // double timestamp = 1;
  if (!(this->timestamp() <= 0 && this->timestamp() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(1, this->_internal_timestamp(), target);
  }

  // double latitude = 2;
  if (!(this->latitude() <= 0 && this->latitude() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(2, this->_internal_latitude(), target);
  }

  // double longitude = 3;
  if (!(this->longitude() <= 0 && this->longitude() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(3, this->_internal_longitude(), target);
  }

  // double altitude = 4;
  if (!(this->altitude() <= 0 && this->altitude() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteDoubleToArray(4, this->_internal_altitude(), target);
  }

  // float roll = 5;
  if (!(this->roll() <= 0 && this->roll() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(5, this->_internal_roll(), target);
  }

  // float pitch = 6;
  if (!(this->pitch() <= 0 && this->pitch() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(6, this->_internal_pitch(), target);
  }

  // float yaw = 7;
  if (!(this->yaw() <= 0 && this->yaw() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(7, this->_internal_yaw(), target);
  }

  // float accel_x = 8;
  if (!(this->accel_x() <= 0 && this->accel_x() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(8, this->_internal_accel_x(), target);
  }

  // float accel_y = 9;
  if (!(this->accel_y() <= 0 && this->accel_y() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(9, this->_internal_accel_y(), target);
  }

  // float accel_z = 10;
  if (!(this->accel_z() <= 0 && this->accel_z() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(10, this->_internal_accel_z(), target);
  }

  // float barometer = 11;
  if (!(this->barometer() <= 0 && this->barometer() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(11, this->_internal_barometer(), target);
  }

  // float course = 12;
  if (!(this->course() <= 0 && this->course() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(12, this->_internal_course(), target);
  }

  // float speed = 13;
  if (!(this->speed() <= 0 && this->speed() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(13, this->_internal_speed(), target);
  }

  // float velocityN = 14;
  if (!(this->velocityn() <= 0 && this->velocityn() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(14, this->_internal_velocityn(), target);
  }

  // float velocityE = 15;
  if (!(this->velocitye() <= 0 && this->velocitye() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(15, this->_internal_velocitye(), target);
  }

  // float velocityD = 16;
  if (!(this->velocityd() <= 0 && this->velocityd() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(16, this->_internal_velocityd(), target);
  }

  // int32 numSats = 17;
  if (this->numsats() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(17, this->_internal_numsats(), target);
  }

  // float declination = 18;
  if (!(this->declination() <= 0 && this->declination() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(18, this->_internal_declination(), target);
  }

  // .raven.fcu.swicd.RavenFCU_SWICD_INSmessage.FixType gpsFix = 19;
  if (this->gpsfix() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      19, this->_internal_gpsfix(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  return target;
}

size_t RavenFCU_SWICD_INSmessage::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // double timestamp = 1;
  if (!(this->timestamp() <= 0 && this->timestamp() >= 0)) {
    total_size += 1 + 8;
  }

  // double latitude = 2;
  if (!(this->latitude() <= 0 && this->latitude() >= 0)) {
    total_size += 1 + 8;
  }

  // double longitude = 3;
  if (!(this->longitude() <= 0 && this->longitude() >= 0)) {
    total_size += 1 + 8;
  }

  // double altitude = 4;
  if (!(this->altitude() <= 0 && this->altitude() >= 0)) {
    total_size += 1 + 8;
  }

  // float roll = 5;
  if (!(this->roll() <= 0 && this->roll() >= 0)) {
    total_size += 1 + 4;
  }

  // float pitch = 6;
  if (!(this->pitch() <= 0 && this->pitch() >= 0)) {
    total_size += 1 + 4;
  }

  // float yaw = 7;
  if (!(this->yaw() <= 0 && this->yaw() >= 0)) {
    total_size += 1 + 4;
  }

  // float accel_x = 8;
  if (!(this->accel_x() <= 0 && this->accel_x() >= 0)) {
    total_size += 1 + 4;
  }

  // float accel_y = 9;
  if (!(this->accel_y() <= 0 && this->accel_y() >= 0)) {
    total_size += 1 + 4;
  }

  // float accel_z = 10;
  if (!(this->accel_z() <= 0 && this->accel_z() >= 0)) {
    total_size += 1 + 4;
  }

  // float barometer = 11;
  if (!(this->barometer() <= 0 && this->barometer() >= 0)) {
    total_size += 1 + 4;
  }

  // float course = 12;
  if (!(this->course() <= 0 && this->course() >= 0)) {
    total_size += 1 + 4;
  }

  // float speed = 13;
  if (!(this->speed() <= 0 && this->speed() >= 0)) {
    total_size += 1 + 4;
  }

  // float velocityN = 14;
  if (!(this->velocityn() <= 0 && this->velocityn() >= 0)) {
    total_size += 1 + 4;
  }

  // float velocityE = 15;
  if (!(this->velocitye() <= 0 && this->velocitye() >= 0)) {
    total_size += 1 + 4;
  }

  // float velocityD = 16;
  if (!(this->velocityd() <= 0 && this->velocityd() >= 0)) {
    total_size += 2 + 4;
  }

  // int32 numSats = 17;
  if (this->numsats() != 0) {
    total_size += 2 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_numsats());
  }

  // float declination = 18;
  if (!(this->declination() <= 0 && this->declination() >= 0)) {
    total_size += 2 + 4;
  }

  // .raven.fcu.swicd.RavenFCU_SWICD_INSmessage.FixType gpsFix = 19;
  if (this->gpsfix() != 0) {
    total_size += 2 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_gpsfix());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void RavenFCU_SWICD_INSmessage::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  GOOGLE_DCHECK_NE(&from, this);
  const RavenFCU_SWICD_INSmessage* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<RavenFCU_SWICD_INSmessage>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
    MergeFrom(*source);
  }
}

void RavenFCU_SWICD_INSmessage::MergeFrom(const RavenFCU_SWICD_INSmessage& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!(from.timestamp() <= 0 && from.timestamp() >= 0)) {
    _internal_set_timestamp(from._internal_timestamp());
  }
  if (!(from.latitude() <= 0 && from.latitude() >= 0)) {
    _internal_set_latitude(from._internal_latitude());
  }
  if (!(from.longitude() <= 0 && from.longitude() >= 0)) {
    _internal_set_longitude(from._internal_longitude());
  }
  if (!(from.altitude() <= 0 && from.altitude() >= 0)) {
    _internal_set_altitude(from._internal_altitude());
  }
  if (!(from.roll() <= 0 && from.roll() >= 0)) {
    _internal_set_roll(from._internal_roll());
  }
  if (!(from.pitch() <= 0 && from.pitch() >= 0)) {
    _internal_set_pitch(from._internal_pitch());
  }
  if (!(from.yaw() <= 0 && from.yaw() >= 0)) {
    _internal_set_yaw(from._internal_yaw());
  }
  if (!(from.accel_x() <= 0 && from.accel_x() >= 0)) {
    _internal_set_accel_x(from._internal_accel_x());
  }
  if (!(from.accel_y() <= 0 && from.accel_y() >= 0)) {
    _internal_set_accel_y(from._internal_accel_y());
  }
  if (!(from.accel_z() <= 0 && from.accel_z() >= 0)) {
    _internal_set_accel_z(from._internal_accel_z());
  }
  if (!(from.barometer() <= 0 && from.barometer() >= 0)) {
    _internal_set_barometer(from._internal_barometer());
  }
  if (!(from.course() <= 0 && from.course() >= 0)) {
    _internal_set_course(from._internal_course());
  }
  if (!(from.speed() <= 0 && from.speed() >= 0)) {
    _internal_set_speed(from._internal_speed());
  }
  if (!(from.velocityn() <= 0 && from.velocityn() >= 0)) {
    _internal_set_velocityn(from._internal_velocityn());
  }
  if (!(from.velocitye() <= 0 && from.velocitye() >= 0)) {
    _internal_set_velocitye(from._internal_velocitye());
  }
  if (!(from.velocityd() <= 0 && from.velocityd() >= 0)) {
    _internal_set_velocityd(from._internal_velocityd());
  }
  if (from.numsats() != 0) {
    _internal_set_numsats(from._internal_numsats());
  }
  if (!(from.declination() <= 0 && from.declination() >= 0)) {
    _internal_set_declination(from._internal_declination());
  }
  if (from.gpsfix() != 0) {
    _internal_set_gpsfix(from._internal_gpsfix());
  }
}

void RavenFCU_SWICD_INSmessage::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void RavenFCU_SWICD_INSmessage::CopyFrom(const RavenFCU_SWICD_INSmessage& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:raven.fcu.swicd.RavenFCU_SWICD_INSmessage)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RavenFCU_SWICD_INSmessage::IsInitialized() const {
  return true;
}

void RavenFCU_SWICD_INSmessage::InternalSwap(RavenFCU_SWICD_INSmessage* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(RavenFCU_SWICD_INSmessage, gpsfix_)
      + sizeof(RavenFCU_SWICD_INSmessage::gpsfix_)
      - PROTOBUF_FIELD_OFFSET(RavenFCU_SWICD_INSmessage, timestamp_)>(
          reinterpret_cast<char*>(&timestamp_),
          reinterpret_cast<char*>(&other->timestamp_));
}

::PROTOBUF_NAMESPACE_ID::Metadata RavenFCU_SWICD_INSmessage::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace swicd
}  // namespace fcu
}  // namespace raven
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::raven::fcu::swicd::RavenFCU_SWICD_INSmessage* Arena::CreateMaybeMessage< ::raven::fcu::swicd::RavenFCU_SWICD_INSmessage >(Arena* arena) {
  return Arena::CreateMessageInternal< ::raven::fcu::swicd::RavenFCU_SWICD_INSmessage >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
