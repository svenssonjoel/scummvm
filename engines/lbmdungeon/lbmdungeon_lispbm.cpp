#include "lbmdungeon/lbmdungeon_lispbm.h"

#include "common/debug.h"
#include "common/textconsole.h"

#include "common/file.h"
#include "common/fs.h"

extern "C" {
#include "lispbm.h"
#include "eval_cps.h"
#include "lbm_image.h"
#include "lbm_channel.h"
#include "lbm_c_interop.h"
#include "lbm_flat_value.h"
#include "extensions/lbm_dyn_lib.h"
}

#define HEAP_SIZE              (1 << 14)   /* 16384 cons cells */
#define GC_STACK_SIZE          256
#define PRINT_STACK_SIZE       256
#define EXTENSION_STORAGE_SIZE 256
#define LBM_MEMORY_BLOCKS      160
#define LBM_MEMORY_SIZE        LBM_MEMORY_SIZE_BLOCKS_TO_WORDS(LBM_MEMORY_BLOCKS)
#define LBM_BITMAP_SIZE        LBM_MEMORY_BITMAP_SIZE(LBM_MEMORY_BLOCKS)
#define IMAGE_STORAGE_WORDS    (128 * 1024 / sizeof(uint32_t))

static lbm_cons_t      s_heap[HEAP_SIZE]  __attribute__ ((aligned (8)));
static lbm_uint        s_memory[LBM_MEMORY_SIZE];
static lbm_uint        s_bitmap[LBM_BITMAP_SIZE];
static lbm_extension_t s_extensions[EXTENSION_STORAGE_SIZE];
static uint32_t        s_image[IMAGE_STORAGE_WORDS];

static void (*s_print_fn)(const char *msg) = nullptr;

static lbm_string_channel_state_t s_load_tok_state;
static lbm_char_channel_t         s_load_tok;
static char                       *s_load_buf = nullptr;
static lbm_cid                    s_load_cid = -1;

void lbmdungeon_lispbm_set_print_fn(void (*fn)(const char *msg)) {
  s_print_fn = fn;
}

void lbmdungeon_lispbm_send_event(int block, int flags) {
  if (!lbm_event_handler_exists()) return;
  lbm_flat_value_t fv;
  if (lbm_start_flatten(&fv, 3 * sizeof(lbm_uint))) {
    f_cons(&fv);
    f_i(&fv, (lbm_int)block);
    f_i(&fv, (lbm_int)flags);
    lbm_finish_flatten(&fv);
    if (!lbm_event(&fv)) {
      lbm_free(fv.buf);
    }
  }
}

static int print_callback(const char *fmt, ...) {
  char buf[1024];
  va_list args;
  va_start(args, fmt);
  int n = vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  debug("lbmdungeon: %s", buf);
  return n;
}

lbm_value ext_print(lbm_value *args, lbm_uint argn) {
  if (argn < 1 || !s_print_fn) return lbm_enc_sym(SYM_NIL);

  char output[1024];

  for (unsigned int i = 0; i < argn; i ++) {
    lbm_value t = args[i];
    char *str;
    if (lbm_is_array_r(t) &&
        lbm_value_is_printable_string(t, &str)) {
      s_print_fn(str);
    } else {
      lbm_print_value(output, 1024, t);
      s_print_fn(output);
    }
  }
  //s_print_fn("\r");
  return lbm_enc_sym(SYM_TRUE);
}

lbm_value ext_dbg_print(lbm_value *args, lbm_uint argn) {
  if (argn < 1 || !s_print_fn) return lbm_enc_sym(SYM_NIL);

  char output[1024];

  for (unsigned int i = 0; i < argn; i ++) {
    lbm_value t = args[i];
    char *str;
    if (lbm_is_array_r(t) &&
        lbm_value_is_printable_string(t, &str)) {
      print_callback("%s",str);
    } else {
      lbm_print_value(output, 1024, t);
      print_callback("%s",output);
    }
  }
  //s_print_fn("\r");
  return lbm_enc_sym(SYM_TRUE);
}

static bool image_write(uint32_t w, int32_t ix, bool is_const_heap) {
  (void)is_const_heap;
  s_image[ix] = w;
  return true;
}



static void ctx_done_callback(eval_context_t *ctx) {
  if (ctx->id == s_load_cid) {
    delete[] s_load_buf;
    s_load_buf = nullptr;
    s_load_cid = -1;
  }
}

static void critical_callback(void) {
  warning("lbmdungeon: LispBM critical error");
}

bool lbmdungeon_lispbm_init(void) {
  if (!lbm_init(s_heap, HEAP_SIZE,
                s_memory, LBM_MEMORY_SIZE,
                s_bitmap, LBM_BITMAP_SIZE,
                GC_STACK_SIZE,
                PRINT_STACK_SIZE,
                s_extensions,
                EXTENSION_STORAGE_SIZE)) {
    warning("lbmdungeon: lbm_init failed");
    return false;
  }

  if (!lbm_eval_init_events(20)) {
    warning("lbmdungeon: lbm_eval_init_events failed");
    return false;
  }

  lbm_set_critical_error_callback(critical_callback);
  lbm_set_ctx_done_callback(ctx_done_callback);
  lbm_set_printf_callback(print_callback);
  lbm_dyn_lib_init();
  lbm_set_dynamic_load_callback(lbm_dyn_lib_find);

  /* Image storage must be initialised before any symbol additions */
  lbm_image_init(s_image, IMAGE_STORAGE_WORDS, image_write);
  lbm_image_create("lbmdungeon");
  if (!lbm_image_boot()) {
    warning("lbmdungeon: lbm_image_boot failed");
    return false;
  }

  lbm_add_eval_symbols();
  lbm_add_extension("print", ext_print);
  lbm_add_extension("dbg-print", ext_dbg_print);

  debug("lbmdungeon: LispBM initialized");
  return true;
}

void lbmdungeon_lispbm_step(void) {
  for (int i = 0; i < 500; i++)
    lbm_eval_step();
}

void lbmdungeon_lispbm_eval(const char *source) {
  lbm_string_channel_state_t tok_state;
  lbm_char_channel_t         tok;
  lbm_create_string_char_channel(&tok_state, &tok, (char *)source);
  lbm_load_and_eval_expression(&tok);
}

void lbmdungeon_lispbm_load_file(const char *path) {
  Common::File f;
  if (!f.open(Common::FSNode(path))) {
    warning("lbmdungeon: cannot open script: %s", path);
    return;
  }
  int32 size = f.size();
  delete[] s_load_buf;
  s_load_buf = new char[size + 1];
  f.read(s_load_buf, size);
  f.close();
  s_load_buf[size] = '\0';

  lbm_create_string_char_channel(&s_load_tok_state, &s_load_tok, s_load_buf);
  s_load_cid = lbm_load_and_eval_program(&s_load_tok, (char *)path);
}
