#include "Project8GameMode.h"
#include "Project8Character.h"
#include "Project8PlayerController.h"
#include "MyGameState.h"

void AProject8GameMode::MyGameMode()
{
	DefaultPawnClass = AProject8Character::StaticClass();
	PlayerControllerClass = AProject8PlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}
