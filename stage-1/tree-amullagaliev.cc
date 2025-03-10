#define INCLUDE_MEMORY
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "tree.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "gimple-iterator.h"
#include "gimple-walk.h"
#include "internal-fn.h"
#include "gimple-pretty-print.h"

// Included for dump_printf:
#include "tree-pretty-print.h"
#include "diagnostic.h"
#include "dumpfile.h"
#include "builtins.h"
//#include <stdlib.h>
namespace {
	const pass_data pass_data_amullagaliev =
	{
		GIMPLE_PASS,
		"amullagaliev",
		OPTGROUP_NONE,
		TV_NONE,
		PROP_cfg,
		0,
		0,
		0,
		0,
	};

	class pass_amullagaliev : public gimple_opt_pass
	{
		public:
			pass_amullagaliev (gcc::context *ctxt)
				: gimple_opt_pass (pass_data_amullagaliev, ctxt)
			{}

			/* opt_pass methods: */

			bool gate (function *) final override {
				return 1; // always execute pass
			}
			unsigned int execute (function *) final override;

	}; // class pass_amullagaliev

	unsigned int
	pass_amullagaliev::execute (function *fun)
	  {
		  basic_block bb;

		  int bb_cnt = 0, stmt_cnt = 0;

		  FOR_EACH_BB_FN (bb, fun)
		  {
			  bb_cnt++;
			  if (dump_file)
			  {
				  fprintf (dump_file, "===== Basic block count: %d =====\n", bb_cnt);
			  }

			  for (gimple_stmt_iterator gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
			  {
				  gimple *g =  gsi_stmt (gsi);
				  stmt_cnt++;
				  if (dump_file)
				  {
					  fprintf (dump_file, "----- Statement count: %d -----\n", stmt_cnt);
					  print_gimple_stmt (dump_file, g, 0, TDF_VOPS|TDF_MEMSYMS);
				  }
			  }
		  }

		  if (dump_file)
		  {
			  fprintf (dump_file, "------------------------------------\n");
			  fprintf (dump_file, "Total Basic Blocks: %d\n", bb_cnt);
			  fprintf (dump_file, "Total Gimple Statements: %d\n", stmt_cnt);
			  fprintf (dump_file, "------------------------------------\n\n");
		  }

		return 0;
	  }
}

gimple_opt_pass *
make_pass_amullagaliev (gcc::context *ctxt)
{
	return new pass_amullagaliev (ctxt);
}
