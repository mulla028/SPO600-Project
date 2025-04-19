/*
 *  Amir Mullagaliev
 *  SPO600 - Stage 3
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
#include "cgraph.h"
#include <string>
#include <string.h>
#include <map>
#include <vector>

namespace {

const pass_data pass_data_amullagaliev = {
    GIMPLE_PASS,
    "amullagaliev",
    OPTGROUP_NONE,
    TV_NONE,
    PROP_gimple_any,
    0,
    0,
    0,
    0
};

struct FunctionVariant {
    std::string full_name;
    size_t block_count;
    size_t statement_count;
};

static std::map<std::string, std::vector<FunctionVariant> > function_variants;

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

bool is_resolver_function(const std::string &fname) {
    return fname.find(".resolver") != std::string::npos;
}

FunctionVariant find_default_variant(const std::vector<FunctionVariant> &variants) {
    for (size_t i = 0; i < variants.size(); i++) {
        if (variants[i].full_name.find(".default") != std::string::npos) {
            return variants[i];
        }
    }

    for (size_t i = 0; i < variants.size(); i++) {
        if (variants[i].full_name.find('.') == std::string::npos) {
            return variants[i];
        }
    }

    return variants[0];
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

void analyze_function_variants(FILE *out, const std::string &base_name,
                              const std::vector<FunctionVariant> &variants) {
    if (variants.size() <= 1) {
        return;
    }

    FunctionVariant default_variant = find_default_variant(variants);

    for (size_t i = 0; i < variants.size(); i++) {
        const FunctionVariant &variant = variants[i];

        if (variant.full_name == default_variant.full_name) {
            continue;
        }

        bool should_prune = (variant.block_count == default_variant.block_count &&
                            variant.statement_count == default_variant.statement_count);

        if (should_prune) {
            fprintf(out, "PRUNE: %s\n", base_name.c_str());
        } else {
            fprintf(out, "NOPRUNE: %s\n", base_name.c_str());
        }
        fprintf(out, "CLONE FOUND: %s\n", base_name.c_str());
        fprintf(out, "CURRENT: %s\n", variant.full_name.c_str());

        std::string border(60, '*');
        fprintf(out, "%s\n", border.c_str());
        fprintf(out, "*  End of Diagnostic for Clone Pair\n");
        fprintf(out, "%s\n", border.c_str());
    }
}

class pass_amullagaliev : public gimple_opt_pass {
public:
    pass_amullagaliev(gcc::context *ctxt)
      : gimple_opt_pass(pass_data_amullagaliev, ctxt) {}

    bool gate(function *fun) { (void)fun; return true; }

    unsigned int execute(function *fun) {
        FILE *out = (dump_file != nullptr) ? dump_file : stderr;

        struct cgraph_node *node = cgraph_node::get(fun->decl);
        std::string full_fname = (node != nullptr)
                               ? std::string(node->name())
                               : std::string(function_name(fun));

        print_frame_header(out, full_fname);

        if (is_resolver_function(full_fname)) {
            print_frame_footer(out, "ANALYSIS FINISHED (resolver function)");
            return 0;
        }

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

        std::string base_name = get_base_function_name(fun);
        FunctionVariant current_variant = {full_fname, bb_count, gimple_count};
        function_variants[base_name].push_back(current_variant);

        if (function_variants[base_name].size() > 1) {
            analyze_function_variants(out, base_name, function_variants[base_name]);
        } else {
            print_frame_footer(out, "First variant of this function - storing for comparison");
        }

        return 0;
    }
};

} // end anonymous namespace

gimple_opt_pass *make_pass_amullagaliev(gcc::context *ctxt)
{
    return new pass_amullagaliev(ctxt);
}
