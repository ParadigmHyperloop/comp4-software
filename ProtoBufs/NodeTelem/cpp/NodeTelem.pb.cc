// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: NodeTelem.proto

#include "NodeTelem.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

namespace fc {
class brakeNodeDataDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<brakeNodeData> _instance;
} _brakeNodeData_default_instance_;
}  // namespace fc
static void InitDefaultsbrakeNodeData_NodeTelem_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::fc::_brakeNodeData_default_instance_;
    new (ptr) ::fc::brakeNodeData();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::fc::brakeNodeData::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_brakeNodeData_NodeTelem_2eproto =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsbrakeNodeData_NodeTelem_2eproto}, {}};

void InitDefaults_NodeTelem_2eproto() {
  ::google::protobuf::internal::InitSCC(&scc_info_brakeNodeData_NodeTelem_2eproto.base);
}

::google::protobuf::Metadata file_level_metadata_NodeTelem_2eproto[1];
const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors_NodeTelem_2eproto[1];
constexpr ::google::protobuf::ServiceDescriptor const** file_level_service_descriptors_NodeTelem_2eproto = nullptr;

const ::google::protobuf::uint32 TableStruct_NodeTelem_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, id_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, state_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, sol1_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, sol2_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, sol3_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, sol4_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, sol5_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, sol6_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, hp_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, lp1_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, lp2_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, lp3_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, lp4_),
  PROTOBUF_FIELD_OFFSET(::fc::brakeNodeData, temp_),
  0,
  13,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
};
static const ::google::protobuf::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 19, sizeof(::fc::brakeNodeData)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::fc::_brakeNodeData_default_instance_),
};

::google::protobuf::internal::AssignDescriptorsTable assign_descriptors_table_NodeTelem_2eproto = {
  {}, AddDescriptors_NodeTelem_2eproto, "NodeTelem.proto", schemas,
  file_default_instances, TableStruct_NodeTelem_2eproto::offsets,
  file_level_metadata_NodeTelem_2eproto, 1, file_level_enum_descriptors_NodeTelem_2eproto, file_level_service_descriptors_NodeTelem_2eproto,
};

const char descriptor_table_protodef_NodeTelem_2eproto[] =
  "\n\017NodeTelem.proto\022\002fc\"\215\003\n\rbrakeNodeData\022"
  "\n\n\002id\030\001 \002(\005\022/\n\005state\030\016 \001(\0162 .fc.brakeNod"
  "eData.breakNodeState\022\014\n\004sol1\030\002 \001(\010\022\014\n\004so"
  "l2\030\003 \001(\010\022\014\n\004sol3\030\004 \001(\010\022\014\n\004sol4\030\005 \001(\010\022\014\n\004"
  "sol5\030\006 \001(\010\022\014\n\004sol6\030\007 \001(\010\022\n\n\002hp\030\010 \001(\005\022\013\n\003"
  "lp1\030\t \001(\005\022\013\n\003lp2\030\n \001(\005\022\013\n\003lp3\030\013 \001(\005\022\013\n\003l"
  "p4\030\014 \001(\005\022\014\n\004temp\030\r \001(\005\"\234\001\n\016breakNodeStat"
  "e\022\016\n\nbnsBooting\020\000\022\016\n\nbnsStandby\020\001\022\r\n\tbns"
  "Arming\020\002\022\014\n\010bnsArmed\020\003\022\r\n\tbnsFlight\020\004\022\016\n"
  "\nbnsBraking\020\005\022\016\n\nbnsVenting\020\006\022\020\n\014bnsRetr"
  "ieval\020\007\022\014\n\010bnsError\020\010"
  ;
::google::protobuf::internal::DescriptorTable descriptor_table_NodeTelem_2eproto = {
  false, InitDefaults_NodeTelem_2eproto, 
  descriptor_table_protodef_NodeTelem_2eproto,
  "NodeTelem.proto", &assign_descriptors_table_NodeTelem_2eproto, 421,
};

void AddDescriptors_NodeTelem_2eproto() {
  static constexpr ::google::protobuf::internal::InitFunc deps[1] =
  {
  };
 ::google::protobuf::internal::AddDescriptors(&descriptor_table_NodeTelem_2eproto, deps, 0);
}

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_NodeTelem_2eproto = []() { AddDescriptors_NodeTelem_2eproto(); return true; }();
namespace fc {
const ::google::protobuf::EnumDescriptor* brakeNodeData_breakNodeState_descriptor() {
  ::google::protobuf::internal::AssignDescriptors(&assign_descriptors_table_NodeTelem_2eproto);
  return file_level_enum_descriptors_NodeTelem_2eproto[0];
}
bool brakeNodeData_breakNodeState_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const brakeNodeData_breakNodeState brakeNodeData::bnsBooting;
const brakeNodeData_breakNodeState brakeNodeData::bnsStandby;
const brakeNodeData_breakNodeState brakeNodeData::bnsArming;
const brakeNodeData_breakNodeState brakeNodeData::bnsArmed;
const brakeNodeData_breakNodeState brakeNodeData::bnsFlight;
const brakeNodeData_breakNodeState brakeNodeData::bnsBraking;
const brakeNodeData_breakNodeState brakeNodeData::bnsVenting;
const brakeNodeData_breakNodeState brakeNodeData::bnsRetrieval;
const brakeNodeData_breakNodeState brakeNodeData::bnsError;
const brakeNodeData_breakNodeState brakeNodeData::breakNodeState_MIN;
const brakeNodeData_breakNodeState brakeNodeData::breakNodeState_MAX;
const int brakeNodeData::breakNodeState_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

// ===================================================================

void brakeNodeData::InitAsDefaultInstance() {
}
class brakeNodeData::HasBitSetters {
 public:
  static void set_has_id(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000001u;
  }
  static void set_has_state(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00002000u;
  }
  static void set_has_sol1(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000002u;
  }
  static void set_has_sol2(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000004u;
  }
  static void set_has_sol3(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000008u;
  }
  static void set_has_sol4(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000010u;
  }
  static void set_has_sol5(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000020u;
  }
  static void set_has_sol6(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000040u;
  }
  static void set_has_hp(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000080u;
  }
  static void set_has_lp1(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000100u;
  }
  static void set_has_lp2(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000200u;
  }
  static void set_has_lp3(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000400u;
  }
  static void set_has_lp4(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00000800u;
  }
  static void set_has_temp(brakeNodeData* msg) {
    msg->_has_bits_[0] |= 0x00001000u;
  }
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int brakeNodeData::kIdFieldNumber;
const int brakeNodeData::kStateFieldNumber;
const int brakeNodeData::kSol1FieldNumber;
const int brakeNodeData::kSol2FieldNumber;
const int brakeNodeData::kSol3FieldNumber;
const int brakeNodeData::kSol4FieldNumber;
const int brakeNodeData::kSol5FieldNumber;
const int brakeNodeData::kSol6FieldNumber;
const int brakeNodeData::kHpFieldNumber;
const int brakeNodeData::kLp1FieldNumber;
const int brakeNodeData::kLp2FieldNumber;
const int brakeNodeData::kLp3FieldNumber;
const int brakeNodeData::kLp4FieldNumber;
const int brakeNodeData::kTempFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

brakeNodeData::brakeNodeData()
  : ::google::protobuf::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:fc.brakeNodeData)
}
brakeNodeData::brakeNodeData(const brakeNodeData& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(nullptr),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::memcpy(&id_, &from.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&state_) -
    reinterpret_cast<char*>(&id_)) + sizeof(state_));
  // @@protoc_insertion_point(copy_constructor:fc.brakeNodeData)
}

void brakeNodeData::SharedCtor() {
  ::memset(&id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&state_) -
      reinterpret_cast<char*>(&id_)) + sizeof(state_));
}

brakeNodeData::~brakeNodeData() {
  // @@protoc_insertion_point(destructor:fc.brakeNodeData)
  SharedDtor();
}

void brakeNodeData::SharedDtor() {
}

void brakeNodeData::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const brakeNodeData& brakeNodeData::default_instance() {
  ::google::protobuf::internal::InitSCC(&::scc_info_brakeNodeData_NodeTelem_2eproto.base);
  return *internal_default_instance();
}


void brakeNodeData::Clear() {
// @@protoc_insertion_point(message_clear_start:fc.brakeNodeData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x000000ffu) {
    ::memset(&id_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&hp_) -
        reinterpret_cast<char*>(&id_)) + sizeof(hp_));
  }
  if (cached_has_bits & 0x00003f00u) {
    ::memset(&lp1_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&state_) -
        reinterpret_cast<char*>(&lp1_)) + sizeof(state_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* brakeNodeData::_InternalParse(const char* begin, const char* end, void* object,
                  ::google::protobuf::internal::ParseContext* ctx) {
  auto msg = static_cast<brakeNodeData*>(object);
  ::google::protobuf::int32 size; (void)size;
  int depth; (void)depth;
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::ParseFunc parser_till_end; (void)parser_till_end;
  auto ptr = begin;
  while (ptr < end) {
    ptr = ::google::protobuf::io::Parse32(ptr, &tag);
    GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
    switch (tag >> 3) {
      // required int32 id = 1;
      case 1: {
        if (static_cast<::google::protobuf::uint8>(tag) != 8) goto handle_unusual;
        msg->set_id(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional bool sol1 = 2;
      case 2: {
        if (static_cast<::google::protobuf::uint8>(tag) != 16) goto handle_unusual;
        msg->set_sol1(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional bool sol2 = 3;
      case 3: {
        if (static_cast<::google::protobuf::uint8>(tag) != 24) goto handle_unusual;
        msg->set_sol2(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional bool sol3 = 4;
      case 4: {
        if (static_cast<::google::protobuf::uint8>(tag) != 32) goto handle_unusual;
        msg->set_sol3(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional bool sol4 = 5;
      case 5: {
        if (static_cast<::google::protobuf::uint8>(tag) != 40) goto handle_unusual;
        msg->set_sol4(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional bool sol5 = 6;
      case 6: {
        if (static_cast<::google::protobuf::uint8>(tag) != 48) goto handle_unusual;
        msg->set_sol5(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional bool sol6 = 7;
      case 7: {
        if (static_cast<::google::protobuf::uint8>(tag) != 56) goto handle_unusual;
        msg->set_sol6(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional int32 hp = 8;
      case 8: {
        if (static_cast<::google::protobuf::uint8>(tag) != 64) goto handle_unusual;
        msg->set_hp(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional int32 lp1 = 9;
      case 9: {
        if (static_cast<::google::protobuf::uint8>(tag) != 72) goto handle_unusual;
        msg->set_lp1(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional int32 lp2 = 10;
      case 10: {
        if (static_cast<::google::protobuf::uint8>(tag) != 80) goto handle_unusual;
        msg->set_lp2(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional int32 lp3 = 11;
      case 11: {
        if (static_cast<::google::protobuf::uint8>(tag) != 88) goto handle_unusual;
        msg->set_lp3(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional int32 lp4 = 12;
      case 12: {
        if (static_cast<::google::protobuf::uint8>(tag) != 96) goto handle_unusual;
        msg->set_lp4(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional int32 temp = 13;
      case 13: {
        if (static_cast<::google::protobuf::uint8>(tag) != 104) goto handle_unusual;
        msg->set_temp(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      // optional .fc.brakeNodeData.breakNodeState state = 14;
      case 14: {
        if (static_cast<::google::protobuf::uint8>(tag) != 112) goto handle_unusual;
        ::google::protobuf::uint64 val = ::google::protobuf::internal::ReadVarint(&ptr);
        if (!::fc::brakeNodeData_breakNodeState_IsValid(val)) {
          ::google::protobuf::internal::WriteVarint(14, val, msg->mutable_unknown_fields());
          break;
        }
        msg->set_state(static_cast<::fc::brakeNodeData_breakNodeState>(val));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->EndGroup(tag);
          return ptr;
        }
        auto res = UnknownFieldParse(tag, {_InternalParse, msg},
          ptr, end, msg->_internal_metadata_.mutable_unknown_fields(), ctx);
        ptr = res.first;
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr != nullptr);
        if (res.second) return ptr;
      }
    }  // switch
  }  // while
  return ptr;
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool brakeNodeData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:fc.brakeNodeData)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 id = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (8 & 0xFF)) {
          HasBitSetters::set_has_id(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool sol1 = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (16 & 0xFF)) {
          HasBitSetters::set_has_sol1(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &sol1_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool sol2 = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (24 & 0xFF)) {
          HasBitSetters::set_has_sol2(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &sol2_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool sol3 = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (32 & 0xFF)) {
          HasBitSetters::set_has_sol3(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &sol3_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool sol4 = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (40 & 0xFF)) {
          HasBitSetters::set_has_sol4(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &sol4_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool sol5 = 6;
      case 6: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (48 & 0xFF)) {
          HasBitSetters::set_has_sol5(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &sol5_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool sol6 = 7;
      case 7: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (56 & 0xFF)) {
          HasBitSetters::set_has_sol6(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &sol6_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 hp = 8;
      case 8: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (64 & 0xFF)) {
          HasBitSetters::set_has_hp(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &hp_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 lp1 = 9;
      case 9: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (72 & 0xFF)) {
          HasBitSetters::set_has_lp1(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &lp1_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 lp2 = 10;
      case 10: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (80 & 0xFF)) {
          HasBitSetters::set_has_lp2(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &lp2_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 lp3 = 11;
      case 11: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (88 & 0xFF)) {
          HasBitSetters::set_has_lp3(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &lp3_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 lp4 = 12;
      case 12: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (96 & 0xFF)) {
          HasBitSetters::set_has_lp4(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &lp4_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 temp = 13;
      case 13: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (104 & 0xFF)) {
          HasBitSetters::set_has_temp(this);
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &temp_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .fc.brakeNodeData.breakNodeState state = 14;
      case 14: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (112 & 0xFF)) {
          int value = 0;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::fc::brakeNodeData_breakNodeState_IsValid(value)) {
            set_state(static_cast< ::fc::brakeNodeData_breakNodeState >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                14, static_cast<::google::protobuf::uint64>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:fc.brakeNodeData)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:fc.brakeNodeData)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void brakeNodeData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:fc.brakeNodeData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required int32 id = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->id(), output);
  }

  // optional bool sol1 = 2;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->sol1(), output);
  }

  // optional bool sol2 = 3;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(3, this->sol2(), output);
  }

  // optional bool sol3 = 4;
  if (cached_has_bits & 0x00000008u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(4, this->sol3(), output);
  }

  // optional bool sol4 = 5;
  if (cached_has_bits & 0x00000010u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(5, this->sol4(), output);
  }

  // optional bool sol5 = 6;
  if (cached_has_bits & 0x00000020u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(6, this->sol5(), output);
  }

  // optional bool sol6 = 7;
  if (cached_has_bits & 0x00000040u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(7, this->sol6(), output);
  }

  // optional int32 hp = 8;
  if (cached_has_bits & 0x00000080u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(8, this->hp(), output);
  }

  // optional int32 lp1 = 9;
  if (cached_has_bits & 0x00000100u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(9, this->lp1(), output);
  }

  // optional int32 lp2 = 10;
  if (cached_has_bits & 0x00000200u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(10, this->lp2(), output);
  }

  // optional int32 lp3 = 11;
  if (cached_has_bits & 0x00000400u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(11, this->lp3(), output);
  }

  // optional int32 lp4 = 12;
  if (cached_has_bits & 0x00000800u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(12, this->lp4(), output);
  }

  // optional int32 temp = 13;
  if (cached_has_bits & 0x00001000u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(13, this->temp(), output);
  }

  // optional .fc.brakeNodeData.breakNodeState state = 14;
  if (cached_has_bits & 0x00002000u) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      14, this->state(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:fc.brakeNodeData)
}

::google::protobuf::uint8* brakeNodeData::InternalSerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:fc.brakeNodeData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required int32 id = 1;
  if (cached_has_bits & 0x00000001u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->id(), target);
  }

  // optional bool sol1 = 2;
  if (cached_has_bits & 0x00000002u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(2, this->sol1(), target);
  }

  // optional bool sol2 = 3;
  if (cached_has_bits & 0x00000004u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(3, this->sol2(), target);
  }

  // optional bool sol3 = 4;
  if (cached_has_bits & 0x00000008u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(4, this->sol3(), target);
  }

  // optional bool sol4 = 5;
  if (cached_has_bits & 0x00000010u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(5, this->sol4(), target);
  }

  // optional bool sol5 = 6;
  if (cached_has_bits & 0x00000020u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(6, this->sol5(), target);
  }

  // optional bool sol6 = 7;
  if (cached_has_bits & 0x00000040u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(7, this->sol6(), target);
  }

  // optional int32 hp = 8;
  if (cached_has_bits & 0x00000080u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(8, this->hp(), target);
  }

  // optional int32 lp1 = 9;
  if (cached_has_bits & 0x00000100u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(9, this->lp1(), target);
  }

  // optional int32 lp2 = 10;
  if (cached_has_bits & 0x00000200u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(10, this->lp2(), target);
  }

  // optional int32 lp3 = 11;
  if (cached_has_bits & 0x00000400u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(11, this->lp3(), target);
  }

  // optional int32 lp4 = 12;
  if (cached_has_bits & 0x00000800u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(12, this->lp4(), target);
  }

  // optional int32 temp = 13;
  if (cached_has_bits & 0x00001000u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(13, this->temp(), target);
  }

  // optional .fc.brakeNodeData.breakNodeState state = 14;
  if (cached_has_bits & 0x00002000u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      14, this->state(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:fc.brakeNodeData)
  return target;
}

size_t brakeNodeData::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:fc.brakeNodeData)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  // required int32 id = 1;
  if (has_id()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->id());
  }
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x000000feu) {
    // optional bool sol1 = 2;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 + 1;
    }

    // optional bool sol2 = 3;
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 + 1;
    }

    // optional bool sol3 = 4;
    if (cached_has_bits & 0x00000008u) {
      total_size += 1 + 1;
    }

    // optional bool sol4 = 5;
    if (cached_has_bits & 0x00000010u) {
      total_size += 1 + 1;
    }

    // optional bool sol5 = 6;
    if (cached_has_bits & 0x00000020u) {
      total_size += 1 + 1;
    }

    // optional bool sol6 = 7;
    if (cached_has_bits & 0x00000040u) {
      total_size += 1 + 1;
    }

    // optional int32 hp = 8;
    if (cached_has_bits & 0x00000080u) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->hp());
    }

  }
  if (cached_has_bits & 0x00003f00u) {
    // optional int32 lp1 = 9;
    if (cached_has_bits & 0x00000100u) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->lp1());
    }

    // optional int32 lp2 = 10;
    if (cached_has_bits & 0x00000200u) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->lp2());
    }

    // optional int32 lp3 = 11;
    if (cached_has_bits & 0x00000400u) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->lp3());
    }

    // optional int32 lp4 = 12;
    if (cached_has_bits & 0x00000800u) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->lp4());
    }

    // optional int32 temp = 13;
    if (cached_has_bits & 0x00001000u) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->temp());
    }

    // optional .fc.brakeNodeData.breakNodeState state = 14;
    if (cached_has_bits & 0x00002000u) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->state());
    }

  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void brakeNodeData::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:fc.brakeNodeData)
  GOOGLE_DCHECK_NE(&from, this);
  const brakeNodeData* source =
      ::google::protobuf::DynamicCastToGenerated<brakeNodeData>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:fc.brakeNodeData)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:fc.brakeNodeData)
    MergeFrom(*source);
  }
}

void brakeNodeData::MergeFrom(const brakeNodeData& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:fc.brakeNodeData)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x000000ffu) {
    if (cached_has_bits & 0x00000001u) {
      id_ = from.id_;
    }
    if (cached_has_bits & 0x00000002u) {
      sol1_ = from.sol1_;
    }
    if (cached_has_bits & 0x00000004u) {
      sol2_ = from.sol2_;
    }
    if (cached_has_bits & 0x00000008u) {
      sol3_ = from.sol3_;
    }
    if (cached_has_bits & 0x00000010u) {
      sol4_ = from.sol4_;
    }
    if (cached_has_bits & 0x00000020u) {
      sol5_ = from.sol5_;
    }
    if (cached_has_bits & 0x00000040u) {
      sol6_ = from.sol6_;
    }
    if (cached_has_bits & 0x00000080u) {
      hp_ = from.hp_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
  if (cached_has_bits & 0x00003f00u) {
    if (cached_has_bits & 0x00000100u) {
      lp1_ = from.lp1_;
    }
    if (cached_has_bits & 0x00000200u) {
      lp2_ = from.lp2_;
    }
    if (cached_has_bits & 0x00000400u) {
      lp3_ = from.lp3_;
    }
    if (cached_has_bits & 0x00000800u) {
      lp4_ = from.lp4_;
    }
    if (cached_has_bits & 0x00001000u) {
      temp_ = from.temp_;
    }
    if (cached_has_bits & 0x00002000u) {
      state_ = from.state_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void brakeNodeData::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:fc.brakeNodeData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void brakeNodeData::CopyFrom(const brakeNodeData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:fc.brakeNodeData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool brakeNodeData::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  return true;
}

void brakeNodeData::Swap(brakeNodeData* other) {
  if (other == this) return;
  InternalSwap(other);
}
void brakeNodeData::InternalSwap(brakeNodeData* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  swap(id_, other->id_);
  swap(sol1_, other->sol1_);
  swap(sol2_, other->sol2_);
  swap(sol3_, other->sol3_);
  swap(sol4_, other->sol4_);
  swap(sol5_, other->sol5_);
  swap(sol6_, other->sol6_);
  swap(hp_, other->hp_);
  swap(lp1_, other->lp1_);
  swap(lp2_, other->lp2_);
  swap(lp3_, other->lp3_);
  swap(lp4_, other->lp4_);
  swap(temp_, other->temp_);
  swap(state_, other->state_);
}

::google::protobuf::Metadata brakeNodeData::GetMetadata() const {
  ::google::protobuf::internal::AssignDescriptors(&::assign_descriptors_table_NodeTelem_2eproto);
  return ::file_level_metadata_NodeTelem_2eproto[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace fc
namespace google {
namespace protobuf {
template<> PROTOBUF_NOINLINE ::fc::brakeNodeData* Arena::CreateMaybeMessage< ::fc::brakeNodeData >(Arena* arena) {
  return Arena::CreateInternal< ::fc::brakeNodeData >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
