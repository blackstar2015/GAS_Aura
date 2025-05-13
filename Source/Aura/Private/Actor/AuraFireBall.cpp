// Copyright by Pradeep Pillai


#include "Actor/AuraFireBall.h"

void AAuraFireBall::OnSphereOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}
