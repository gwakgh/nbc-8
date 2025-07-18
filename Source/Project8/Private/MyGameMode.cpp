#include "MyGameMode.h"
#include "Project8Character.h"
#include "Project8PlayerController.h"
#include "MyGameState.h"

void AMyGameMode::SpartaGameMode()
{
	DefaultPawnClass = AProject8Character::StaticClass();
	PlayerControllerClass = AProject8PlayerController::StaticClass();
	GameStateClass = AMyGameState::StaticClass();
}
