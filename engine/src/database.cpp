
#include "database.h"


b32 Database::Manager::init() {
	return true;
}

void Database::Manager::shutdown() {

}

b32 Database::Manip::fetchCellRecord(CellRecord& rec) {
	return true;
}

b32 Database::Manip::saveCellRecord(CellRecord& rec) {
	return true;
}

b32 Database::Manip::deleteCellRecord(CellRecord& rec) {
	return true;
}

// Actor record manipulation
b32 Database::Manip::fetchActorRecord(ActorRecord& rec) {
	return true;
}

b32 Database::Manip::saveActorRecord(ActorRecord& rec) {
	return true;
}

b32 Database::Manip::deleteActorRecord(ActorRecord& rec) {
	return true;
}