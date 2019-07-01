cmake_minimum_required(VERSION 3.6)
project (black_telephone)

set(cmake_cxx_standard 17)

set(BOX2D_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/Shapes/b2ChainShape.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/Shapes/b2CircleShape.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/Shapes/b2EdgeShape.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/Shapes/b2PolygonShape.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2BroadPhase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2CollideCircle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2CollideEdge.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2CollidePolygon.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2Collision.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2Distance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2DynamicTree.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Collision/b2TimeOfImpact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Common/b2BlockAllocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Common/b2Draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Common/b2Math.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Common/b2Settings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Common/b2StackAllocator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Common/b2Timer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2CircleContact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2Contact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2ContactSolver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Contacts/b2PolygonContact.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2DistanceJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2FrictionJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2GearJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2Joint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2MotorJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2MouseJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2PulleyJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2RopeJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2WeldJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/Joints/b2WheelJoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/b2Body.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/b2ContactManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/b2Fixture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/b2Island.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/b2World.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Dynamics/b2WorldCallbacks.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dep/Box2D/Rope/b2Rope.cpp
)

set(BLACK_TELEPHONE_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/sprite_cache.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/sprite_renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/frame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/sim.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/config.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/registry.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/physics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/input.cpp
	)

add_executable(black_telephone main.cpp ${BLACK_TELEPHONE_SRCS} ${BOX2D_SRCS})

set_property(TARGET black_telephone PROPERTY CXX_STANDARD 17)
set_property(TARGET black_telephone PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(black_telephone
	opengl32.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL2/lib/SDL2.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dep/SDL2/lib/SDL2main.lib
	)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/dep
	${CMAKE_CURRENT_SOURCE_DIR}/engine
	${CMAKE_CURRENT_SOURCE_DIR}/utility
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

set_target_properties(black_telephone PROPERTIES COMPILE_FLAGS "/MD")