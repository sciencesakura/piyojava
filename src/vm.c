#include "piyojava.h"
#include "util/stack.h"

typedef enum Opcode Opcode;
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
  OPCODE_istore = 0x36,
  OPCODE_istore_0 = 0x3b,
  OPCODE_istore_1 = 0x3c,
  OPCODE_istore_2 = 0x3d,
  OPCODE_istore_3 = 0x3e,
  OPCODE_return = 0xb1,
};

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

static void _istore_n(Frame *frame, u1 index)
{
  frame->variables[index] = stack_ipop(&frame->operands);
}

static void _istore(Frame *frame)
{
  _istore_n(frame, nextcode(frame));
}

void execute(Frame *frame)
{
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
    case OPCODE_istore:
      _istore(frame);
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
    case OPCODE_return:
      return;
    default:
      error(L"unknown instruction(pc=%" PRIu32 ", opcode=0x%02x)", frame->pc - 1, opcode);
    }
  }
}

