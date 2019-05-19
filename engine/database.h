#pragma once

#include "common.h"

#define DATABASE_MANIPULABLE
#define DATABASE_FORCE_RENEW // DANGEROUS, remakes the database each run. So not leave defined if actually trying to keep a database

#include <string>

namespace Database {
	
	class Manager {
	public:
		b32 init();
		void shutdown();
	};

	struct CellRecord {
		std::string name;
	};

	struct ActorRecord {
		std::string name;
	};

	namespace Manip {
		// Cell record manipulation
		b32 fetchCellRecord(CellRecord& rec);
		b32 saveCellRecord(CellRecord& rec);
		b32 deleteCellRecord(CellRecord& rec);
		// Actor record manipulation
		b32 fetchActorRecord(ActorRecord& rec);
		b32 saveActorRecord(ActorRecord& rec);
		b32 deleteActorRecord(ActorRecord& rec);
	}

}