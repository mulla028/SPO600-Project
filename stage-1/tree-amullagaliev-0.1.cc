/* Test pass
 Amir Mullagaliev, Seneca Polytechnic College, 2025-03
*/

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "tree-pretty-print.h"
#include "gimple-iterator.h"
#include "gimple-walk.h"
#include "internal-fn.h"
#include "gimple-pretty-print.h"

// Added headers:
#include "gimple-ssa.h"
#include "cgraph.h"
#include "attribs.h"
#include "pretty-print.h"
#include "tree-inline.h"
#include "intl.h"

// Test pass

namespace {

const pass_data pass_data_amullagaliev =
{
  GIMPLE_PASS, /* type */
  "amullagaliev", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_TREE_NRV, /* tv_id */
  PROP_cfg , /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_amullagaliev : public gimple_opt_pass
{
public:
  pass_amullagaliev (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_amullagaliev, ctxt)
  {}

  /* opt_pass methods: */
  bool gate (function *) final override {
	  return 1;
  }

  unsigned int execute (function *) final override;

}; // class pass_amullagaliev

unsigned int
pass_amullagaliev::execute (function *) {
	struct cgraph_node *node;
	int func_cnt = 0;

	FOR_EACH_FUNCTION (node)
	  {
		if (dump_file)
		{
			fprintf(dump_file, "=== FUnction %d Name '%s' ===\n", ++func_cnt, node->name() );
		}
	  }

	if (dump_file)
	{
		fprintf(dump_file, "\n\n#### End amullagaliev diagnostics, start regular dump of current gimple ####\n\n\n");
	}

	return 0;  // Moved return statement outside the if block to ensure it always returns
}

} // anonymous namespace

// Factory function that creates an instance of the pass
gimple_opt_pass *
make_pass_amullagaliev (gcc::context *ctxt)
{
  return new pass_amullagaliev (ctxt);
}
