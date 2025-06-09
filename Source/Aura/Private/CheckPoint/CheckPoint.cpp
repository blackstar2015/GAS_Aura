// Copyright by Pradeep Pillai


#include "CheckPoint/CheckPoint.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Aura/Aura.h"


ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;


	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Checkpoint Mesh"));
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckpointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	CheckpointMesh->MarkRenderStateDirty();
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(CheckpointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	MoveToComponent  = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent());
}
//Save Interface Function Implementations
void ACheckPoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}
//Highlight Interface Function Implementations
void ACheckPoint::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void ACheckPoint::UnHighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

void ACheckPoint::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	OutDestination = MoveToComponent->GetComponentLocation();
}

void ACheckPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if(AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			const UWorld* World = GetWorld();
			FString MapName = World->GetMapName();
			MapName.RemoveFromStart(World->StreamingLevelsPrefix);
			
			AuraGameMode->SaveWorldState(GetWorld(), MapName);
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
}

void ACheckPoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterialInstance);
	CheckpointReached(DynamicMaterialInstance);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	if (bBindOverlapCallback) Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereOverlap);	
}
