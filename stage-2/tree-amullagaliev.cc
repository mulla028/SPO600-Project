/*
 *  Amir Mullagaliev, Seneca Polytechnic College, 2025-03
 *  Stage - 2
 */
#define INCLUDE_MEMORY
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "basic-block.h"
#include "function.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "tree-pass.h"
#include "context.h"
#include "cgraph.h"       // for cgraph_node
#include <string>         // for std::string
#include <string.h>       // for strncpy, strstr, etc.

namespace {

const pass_data pass_data_amullagaliev = {
    GIMPLE_PASS,    // type
    "amullagaliev", // name shown in dump flags (-fdump-tree-amullagaliev)
    OPTGROUP_NONE,  // optinfo_flags
    TV_NONE,        // tv_id
    PROP_gimple_any, // properties_required
    0,              // properties_provided
    0,              // properties_destroyed
    0,              // todo_flags_start
    0               // todo_flags_finish
};

static std::string previous_function_name = "";
static size_t previous_block_total = 0;
static size_t previous_statement_total = 0;
std::string get_base_function_name(function *fun) {
    struct cgraph_node *node = cgraph_node::get(fun->decl);
    std::string fname = (node != nullptr) ? std::string(node->name())
                                            : std::string(function_name(fun));
    size_t pos = fname.find(".resolver");
    if (pos != std::string::npos)
        return fname.substr(0, pos);
    pos = fname.find('.');
    if (pos != std::string::npos)
        return fname.substr(0, pos);
    return fname;
}

void print_frame_header(FILE *out, const std::string &funcName) {
    std::string border(60, '*');
    fprintf(out, "\n%s\n", border.c_str());
    fprintf(out, "*  ANALYZATION - Examining Function: %-28s *\n", funcName.c_str());
    fprintf(out, "%s\n", border.c_str());
}

void print_frame_footer(FILE *out, const std::string &message) {
    std::string border(60, '*');
    fprintf(out, "%s\n", border.c_str());
    fprintf(out, "*  %s\n", message.c_str());
    fprintf(out, "%s\n", border.c_str());
}

class pass_amullagaliev : public gimple_opt_pass {
public:
    pass_amullagaliev(gcc::context *ctxt)
      : gimple_opt_pass(pass_data_amullagaliev, ctxt) {}

    bool gate(function *fun) { (void)fun; return true; }

    unsigned int execute(function *fun) {
        // Use a local file pointer that falls back to stderr if dump_file is NULL.
        FILE *out = (dump_file != nullptr) ? dump_file : stderr;

        // Obtain full function name via cgraph if available.
        struct cgraph_node *node = cgraph_node::get(fun->decl);
        std::string full_fname = (node != nullptr)
                                    ? std::string(node->name())
                                    : std::string(function_name(fun));

        print_frame_header(out, full_fname);

        // Count Basic Blocks and GIMPLE statements.
        size_t bb_count = 0;
        size_t gimple_count = 0;
        basic_block bb;
        FOR_EACH_BB_FN(bb, fun) {
            bb_count++;
            for (gimple_stmt_iterator gsi = gsi_start_bb(bb);
                 !gsi_end_p(gsi);
                 gsi_next(&gsi))
            {
                gimple_count++;
            }
        }

        bool is_resolver = (full_fname.find(".resolver") != std::string::npos);
        if (is_resolver) {
            print_frame_footer(out, "ANALYSIS FINISHED (resolver function)");
            return 0;
        }

        // For non-resolver functions, derive the base function name.
        std::string base_name = get_base_function_name(fun);

        if (previous_function_name.empty()) {
            previous_function_name = base_name;
            previous_block_total = bb_count;
            previous_statement_total = gimple_count;
            print_frame_footer(out, "ANALYSIS FINISHED!");
        } else {
            if (previous_function_name == base_name &&
                previous_block_total == bb_count &&
                previous_statement_total == gimple_count)
            {
                fprintf(out, "PRUNE: %s\n", previous_function_name.c_str());
                fprintf(out, "CLONE FOUND: %s\n", previous_function_name.c_str());
                fprintf(out, "CURRENT: %s\n", full_fname.c_str());
            } else {
                fprintf(out, "NOPRUNE: %s\n", previous_function_name.c_str());
                fprintf(out, "CLONE FOUND: %s\n", previous_function_name.c_str());
                fprintf(out, "CURRENT: %s\n", full_fname.c_str());
            }
            print_frame_footer(out, "End of Diagnostic");
            previous_function_name = "";
            previous_block_total = 0;
            previous_statement_total = 0;
        }
        return 0;
    }
};

} // end anonymous namespace

gimple_opt_pass *make_pass_amullagaliev(gcc::context *ctxt)
{
    return new pass_amullagaliev(ctxt);
}
