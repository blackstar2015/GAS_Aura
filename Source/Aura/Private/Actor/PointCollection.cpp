// Copyright by Pradeep Pillai


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
 	PrimaryActorTick.bCanEverTick = false;

#pragma region CreatDefaultSubobjects for points
	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");
#pragma endregion

#pragma region Add to ImmutablePoints Array
	ImmutablePoints.Add(Pt_0);
	ImmutablePoints.Add(Pt_1);
	ImmutablePoints.Add(Pt_2);
	ImmutablePoints.Add(Pt_3);
	ImmutablePoints.Add(Pt_4);
	ImmutablePoints.Add(Pt_5);
	ImmutablePoints.Add(Pt_6);
	ImmutablePoints.Add(Pt_7);
	ImmutablePoints.Add(Pt_8);
	ImmutablePoints.Add(Pt_9);
	ImmutablePoints.Add(Pt_10);
#pragma endregion

#pragma region SetupAttachments
	SetRootComponent(Pt_0);
	Pt_1->SetupAttachment(GetRootComponent());
	Pt_2->SetupAttachment(GetRootComponent());
	Pt_3->SetupAttachment(GetRootComponent());
	Pt_4->SetupAttachment(GetRootComponent());
	Pt_5->SetupAttachment(GetRootComponent());
	Pt_6->SetupAttachment(GetRootComponent());
	Pt_7->SetupAttachment(GetRootComponent());
	Pt_8->SetupAttachment(GetRootComponent());
	Pt_9->SetupAttachment(GetRootComponent());
	Pt_10->SetupAttachment(GetRootComponent());
#pragma endregion
	
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverride)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("Attempted to access ImmutablePts out of bounds."));

	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Pt : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride,FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}
		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this,
			IgnoreActors, TArray<AActor*>(), 1500.f,
			GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult,RaisedLocation, LoweredLocation, FName("BlockAll"),QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
