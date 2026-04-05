#ifndef LBMDUNGEON_LISPBM_H
#define LBMDUNGEON_LISPBM_H

namespace Kyra { class EoBEngine; }

bool lbmdungeon_lispbm_init(void);
void lbmdungeon_lispbm_set_engine(Kyra::EoBEngine *engine);
void lbmdungeon_lispbm_set_print_fn(void (*fn)(const char *msg));
void lbmdungeon_lispbm_send_event(int block, int flags);
void lbmdungeon_lispbm_load_file(const char *path);
void lbmdungeon_lispbm_step(void);
void lbmdungeon_lispbm_eval(const char *source);

#endif // LBMDUNGEON_LISPBM_H
