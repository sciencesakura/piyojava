#include "piyojava.h"
#include "util/hashtable.h"
#include "util/stack.h"

typedef enum Opcode Opcode;
typedef struct JObject JObject;

enum Opcode {
  OPCODE_nop = 0x00,
  OPCODE_iconst_m1 = 0x02,
  OPCODE_iconst_0 = 0x03,
  OPCODE_iconst_1 = 0x04,
  OPCODE_iconst_2 = 0x05,
  OPCODE_iconst_3 = 0x06,
  OPCODE_iconst_4 = 0x07,
  OPCODE_iconst_5 = 0x08,
  OPCODE_bipush = 0x10,
  OPCODE_sipush = 0x11,
  OPCODE_ldc = 0x12,
  OPCODE_iload = 0x15,
  OPCODE_aload = 0x19,
  OPCODE_iload_0 = 0x1a,
  OPCODE_iload_1 = 0x1b,
  OPCODE_iload_2 = 0x1c,
  OPCODE_iload_3 = 0x1d,
  OPCODE_aload_0 = 0x2a,
  OPCODE_aload_1 = 0x2b,
  OPCODE_aload_2 = 0x2c,
  OPCODE_aload_3 = 0x2d,
  OPCODE_istore = 0x36,
  OPCODE_astore = 0x3a,
  OPCODE_istore_0 = 0x3b,
  OPCODE_istore_1 = 0x3c,
  OPCODE_istore_2 = 0x3d,
  OPCODE_istore_3 = 0x3e,
  OPCODE_astore_0 = 0x4b,
  OPCODE_astore_1 = 0x4c,
  OPCODE_astore_2 = 0x4d,
  OPCODE_astore_3 = 0x4e,
  OPCODE_dup = 0x59,
  OPCODE_dup_x1 = 0x5a,
  OPCODE_dup_x2 = 0x5b,
  OPCODE_iadd = 0x60,
  OPCODE_isub = 0x64,
  OPCODE_imul = 0x68,
  OPCODE_idev = 0x6c,
  OPCODE_irem = 0x70,
  OPCODE_ineg = 0x74,
  OPCODE_iinc = 0x84,
  OPCODE_ireturn = 0xac,
  OPCODE_return = 0xb1,
  OPCODE_getstatic = 0xb2,
  OPCODE_putstatic = 0xb3,
  OPCODE_invokevirtual = 0xb6,
  OPCODE_invokespecial = 0xb7,
  OPCODE_invokestatic = 0xb8,
  OPCODE_new = 0xbb,
};

struct JObject {
  HashTable fields;
};

const CONSTANT_NameAndType_info *CLINIT_NAT
    = &(CONSTANT_NameAndType_info) { CONSTANT_NameAndType,
                                     0,
                                     &(CONSTANT_Utf8_info) { CONSTANT_Utf8, 8, (u1 *)"<clinit>" },
                                     0,
                                     &(CONSTANT_Utf8_info) { CONSTANT_Utf8, 3, (u1 *)"()V" } };

#define APPEND_8BIT(a, b) (((a) << 8) | (b))

static inline u1 nextcode(Frame *frame)
{
  return frame->code->code[frame->pc++];
}

static void _iconst(Frame *frame, jint i)
{
  stack_ipush(&frame->operands, i);
}

static void _bipush(Frame *frame)
{
  jint byte = (int8_t)nextcode(frame);
  stack_ipush(&frame->operands, byte);
}

static void _sipush(Frame *frame)
{
  u2 byte1 = nextcode(frame);
  u2 byte2 = nextcode(frame);
  jint value = (int16_t)APPEND_8BIT(byte1, byte2);
  stack_ipush(&frame->operands, value);
}

static void _ldc(Frame *frame)
{
  u1 index = nextcode(frame);
  Cp_info *tc = cp(frame->constant_pool, index);
  switch (tc->tag) {
  case CONSTANT_Integer: {
    CONSTANT_Integer_info *c = (CONSTANT_Integer_info *)tc;
    stack_ipush(&frame->operands, c->value);
    break;
  }
  default:
    error(L"ldc tag(%d) not supported", tc->tag);
  }
}

static void _iload_n(Frame *frame, u1 n)
{
  stack_ipush(&frame->operands, frame->variables[n]);
}

static void _iload(Frame *frame)
{
  _iload_n(frame, nextcode(frame));
}

static void _aload_n(Frame *frame, u1 n)
{
  stack_ipush(&frame->operands, frame->variables[n]);
}

static void _aload(Frame *frame)
{
  _aload_n(frame, nextcode(frame));
}

static void _istore_n(Frame *frame, u1 n)
{
  frame->variables[n] = stack_ipop(&frame->operands);
}

static void _istore(Frame *frame)
{
  _istore_n(frame, nextcode(frame));
}

static void _astore_n(Frame *frame, u1 n)
{
  frame->variables[n] = stack_ipop(&frame->operands);
}

static void _astore(Frame *frame)
{
  _astore_n(frame, nextcode(frame));
}

static void _dup(Frame *frame)
{
  stack_ipush(&frame->operands, stack_ipeek(frame->operands));
}

static void _dup_x1(Frame *frame)
{
  intptr_t value1 = stack_ipop(&frame->operands);
  intptr_t value2 = stack_ipop(&frame->operands);
  stack_ipush(&frame->operands, value1);
  stack_ipush(&frame->operands, value2);
  stack_ipush(&frame->operands, value1);
}

static void _dup_x2(Frame *frame)
{
  intptr_t value1 = stack_ipop(&frame->operands);
  intptr_t value2 = stack_ipop(&frame->operands);
  intptr_t value3 = stack_ipop(&frame->operands);
  stack_ipush(&frame->operands, value1);
  stack_ipush(&frame->operands, value3);
  stack_ipush(&frame->operands, value2);
  stack_ipush(&frame->operands, value1);
}

static void _iadd(Frame *frame)
{
  jint value2 = stack_ipop(&frame->operands);
  jint value1 = stack_ipop(&frame->operands);
  stack_ipush(&frame->operands, value1 + value2);
}

static void _isub(Frame *frame)
{
  jint value2 = stack_ipop(&frame->operands);
  jint value1 = stack_ipop(&frame->operands);
  stack_ipush(&frame->operands, value1 - value2);
}

static void _imul(Frame *frame)
{
  jint value2 = stack_ipop(&frame->operands);
  jint value1 = stack_ipop(&frame->operands);
  stack_ipush(&frame->operands, value1 * value2);
}

static void _idiv(Frame *frame)
{
  jint value2 = stack_ipop(&frame->operands);
  jint value1 = stack_ipop(&frame->operands);
  if (value2 == 0) {
    error(L"division by zero");
  }
  stack_ipush(&frame->operands, value1 / value2);
}

static void _irem(Frame *frame)
{
  jint value2 = stack_ipop(&frame->operands);
  jint value1 = stack_ipop(&frame->operands);
  if (value2 == 0) {
    error(L"division by zero");
  }
  stack_ipush(&frame->operands, value1 % value2);
}

static void _ineg(Frame *frame)
{
  jint value = stack_ipop(&frame->operands);
  stack_ipush(&frame->operands, -value);
}

static void _iinc(Frame *frame)
{
  u1 index = nextcode(frame);
  jint value = frame->variables[index];
  frame->variables[index] = value + (int8_t)nextcode(frame);
}

static void _ireturn(const intptr_t *vmstack)
{
  Frame *frame = stack_peek(vmstack);
  Frame *invoker = stack_peek(vmstack, 1);
  stack_push(&invoker->operands, stack_pop(&frame->operands));
}

static void _getstatic(intptr_t *vmstack)
{
  Frame *frame = stack_peek(vmstack);
  u2 indexbyte1 = nextcode(frame);
  u2 indexbyte2 = nextcode(frame);
  u2 index = APPEND_8BIT(indexbyte1, indexbyte2);
  CONSTANT_Fieldref_info *fref = cp(frame->constant_pool, index);
  ClassFile *cf = load_class(vmstack, fref->class->name);
  Field_info *fi = find_field(cf, fref->name_and_type);
  stack_ipush(&frame->operands, fi->staticval);
}

static void _putstatic(intptr_t *vmstack)
{
  Frame *frame = stack_peek(vmstack);
  u2 indexbyte1 = nextcode(frame);
  u2 indexbyte2 = nextcode(frame);
  u2 index = APPEND_8BIT(indexbyte1, indexbyte2);
  CONSTANT_Fieldref_info *fref = cp(frame->constant_pool, index);
  ClassFile *cf = load_class(vmstack, fref->class->name);
  Field_info *fi = find_field(cf, fref->name_and_type);
  fi->staticval = stack_ipop(&frame->operands);
}

static void _invokevirtual(intptr_t *vmstack)
{
  Frame *frame = stack_peek(vmstack);
  u2 indexbyte1 = nextcode(frame);
  u2 indexbyte2 = nextcode(frame);
  u2 index = APPEND_8BIT(indexbyte1, indexbyte2);
  CONSTANT_Methodref_info *mref = cp(frame->constant_pool, index);
  ClassFile *cf = load_class(vmstack, mref->class->name);
  Method_info *me = find_method(cf, mref->name_and_type);
  Code_attribute *code = code_attr(me);
  intptr_t variables[code->max_locals];
  for (u1 i = me->args_size; i != 0; i--) {
    variables[i] = stack_ipop(&frame->operands);
  }
  variables[0] = stack_ipop(&frame->operands);
  intptr_t operands[code->max_stack];
  stack_push(&vmstack, &(Frame) { 0, code, variables, operands, cf->constant_pool });
  execute(vmstack);
  stack_pop(&vmstack);
}

static void _invokespecial(intptr_t *vmstack)
{
  _invokevirtual(vmstack); // TODO 暫定
}

static void _invokestatic(intptr_t *vmstack)
{
  Frame *frame = stack_peek(vmstack);
  u2 indexbyte1 = nextcode(frame);
  u2 indexbyte2 = nextcode(frame);
  u2 index = APPEND_8BIT(indexbyte1, indexbyte2);
  CONSTANT_Methodref_info *mref = cp(frame->constant_pool, index);
  ClassFile *cf = load_class(vmstack, mref->class->name);
  Method_info *me = find_method(cf, mref->name_and_type);
  if (!(me->access_flags & ME_ACC_STATIC)) {
    error(L"method must be static - invokestatic(pc=%" PRIu32 ")", frame->pc);
  }
  Code_attribute *code = code_attr(me);
  intptr_t variables[code->max_locals];
  for (u1 i = me->args_size; i != 0; i--) {
    variables[i - 1] = stack_ipop(&frame->operands);
  }
  intptr_t operands[code->max_stack];
  stack_push(&vmstack, &(Frame) { 0, code, variables, operands, cf->constant_pool });
  execute(vmstack);
  stack_pop(&vmstack);
}

static void _new(intptr_t *vmstack)
{
  Frame *frame = stack_peek(vmstack);
  u2 indexbyte1 = nextcode(frame);
  u2 indexbyte2 = nextcode(frame);
  u2 index = APPEND_8BIT(indexbyte1, indexbyte2);
  CONSTANT_Class_info *cinf = cp(frame->constant_pool, index);
  ClassFile *cf = load_class(vmstack, cinf->name);
  JObject *obj = malloc(sizeof(JObject));
  hashtable_init(&obj->fields, cf->fields_count * 2, utf8hash, utf8eq);
  for (u2 i = 0; i < cf->fields_count; i++) {
    Field_info *fi = &cf->fields[i];
    if (fi->access_flags & FI_ACC_STATIC)
      continue;
    switch (*fi->descriptor->bytes) {
    case 'B':
    case 'C':
    case 'I':
    case 'J':
    case 'S':
    case 'Z':
      hashtable_iput(&obj->fields, fi->name, 0);
      break;
    case 'D':
    case 'F':
      error(L"floating point number is unsupported");
    default:
      hashtable_put(&obj->fields, fi->name, NULL);
    }
  }
  stack_push(&frame->operands, obj);
}

void execute(intptr_t *vmstack)
{
  Frame *frame = stack_peek(vmstack);
  while (frame->pc < frame->code->code_length) {
    Opcode opcode = nextcode(frame);
    debug(L"pc=%" PRIu32 ", opcode=0x%02x", frame->pc - 1, opcode);
    switch (opcode) {
    case OPCODE_nop:
      break;
    case OPCODE_iconst_m1:
      _iconst(frame, -1);
      break;
    case OPCODE_iconst_0:
      _iconst(frame, 0);
      break;
    case OPCODE_iconst_1:
      _iconst(frame, 1);
      break;
    case OPCODE_iconst_2:
      _iconst(frame, 2);
      break;
    case OPCODE_iconst_3:
      _iconst(frame, 3);
      break;
    case OPCODE_iconst_4:
      _iconst(frame, 4);
      break;
    case OPCODE_iconst_5:
      _iconst(frame, 5);
      break;
    case OPCODE_bipush:
      _bipush(frame);
      break;
    case OPCODE_sipush:
      _sipush(frame);
      break;
    case OPCODE_ldc:
      _ldc(frame);
      break;
    case OPCODE_iload:
      _iload(frame);
      break;
    case OPCODE_aload:
      _aload(frame);
      break;
    case OPCODE_iload_0:
      _iload_n(frame, 0);
      break;
    case OPCODE_iload_1:
      _iload_n(frame, 1);
      break;
    case OPCODE_iload_2:
      _iload_n(frame, 2);
      break;
    case OPCODE_iload_3:
      _iload_n(frame, 3);
      break;
    case OPCODE_aload_0:
      _aload_n(frame, 0);
      break;
    case OPCODE_aload_1:
      _aload_n(frame, 1);
      break;
    case OPCODE_aload_2:
      _aload_n(frame, 2);
      break;
    case OPCODE_aload_3:
      _aload_n(frame, 3);
      break;
    case OPCODE_istore:
      _istore(frame);
      break;
    case OPCODE_astore:
      _astore(frame);
      break;
    case OPCODE_istore_0:
      _istore_n(frame, 0);
      break;
    case OPCODE_istore_1:
      _istore_n(frame, 1);
      break;
    case OPCODE_istore_2:
      _istore_n(frame, 2);
      break;
    case OPCODE_istore_3:
      _istore_n(frame, 3);
      break;
    case OPCODE_astore_0:
      _astore_n(frame, 0);
      break;
    case OPCODE_astore_1:
      _astore_n(frame, 1);
      break;
    case OPCODE_astore_2:
      _astore_n(frame, 2);
      break;
    case OPCODE_astore_3:
      _astore_n(frame, 3);
      break;
    case OPCODE_dup:
      _dup(frame);
      break;
    case OPCODE_dup_x1:
      _dup_x1(frame);
      break;
    case OPCODE_dup_x2:
      _dup_x2(frame);
      break;
    case OPCODE_iadd:
      _iadd(frame);
      break;
    case OPCODE_isub:
      _isub(frame);
      break;
    case OPCODE_imul:
      _imul(frame);
      break;
    case OPCODE_idev:
      _idiv(frame);
      break;
    case OPCODE_irem:
      _irem(frame);
      break;
    case OPCODE_ineg:
      _ineg(frame);
      break;
    case OPCODE_iinc:
      _iinc(frame);
      break;
    case OPCODE_ireturn:
      _ireturn(vmstack);
      return;
    case OPCODE_return:
      return;
    case OPCODE_getstatic:
      _getstatic(vmstack);
      break;
    case OPCODE_putstatic:
      _putstatic(vmstack);
      break;
    case OPCODE_invokevirtual:
      _invokevirtual(vmstack);
      break;
    case OPCODE_invokespecial:
      _invokespecial(vmstack);
      break;
    case OPCODE_invokestatic:
      _invokestatic(vmstack);
      break;
    case OPCODE_new:
      _new(vmstack);
      break;
    default:
      error(L"unknown instruction(pc=%" PRIu32 ", opcode=0x%02x)", frame->pc - 1, opcode);
    }
  }
}

ClassFile *load_class(intptr_t *vmstack, CONSTANT_Utf8_info *name)
{
  ClassFile *cf = hashtable_get(&classes, name);
  if (cf != NULL)
    return cf;
  cf = parse_class(name->length, name->bytes);
  hashtable_put(&classes, name, cf);
  for (u2 i = 0; i < cf->fields_count; i++) {
    Field_info *fi = &cf->fields[i];
    if (!(fi->access_flags & FI_ACC_STATIC))
      continue;
    switch (*fi->descriptor->bytes) {
    case 'B':
    case 'C':
    case 'I':
    case 'J':
    case 'S':
    case 'Z':
      fi->staticval = 0;
      break;
    case 'D':
    case 'F':
      error(L"floating point number is unsupported");
    default:
      fi->staticval = (intptr_t)NULL;
      break;
    }
  }
  Method_info *clinit = find_method(cf, CLINIT_NAT);
  if (clinit == NULL || !(clinit->access_flags & ME_ACC_STATIC))
    return cf;
  Code_attribute *code = code_attr(clinit);
  intptr_t variables[code->max_locals];
  intptr_t operands[code->max_stack];
  stack_push(&vmstack, &(Frame) { 0, code, variables, operands, cf->constant_pool });
  execute(vmstack);
  stack_pop(&vmstack);
  return cf;
}
