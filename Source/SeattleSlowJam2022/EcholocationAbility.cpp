// Fill out your copyright notice in the Description page of Project Settings.


#include "EcholocationAbility.h"
#include "AudioActorBase.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UEcholocationAbility::UEcholocationAbility()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEcholocationAbility::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEcholocationAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}


void UEcholocationAbility::OnEcholocationBegin_Implementation()
{
	const int LineCastArcCount = 3;

	APlayerCameraManager *CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (!CamManager)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No CameraManager found!"));
		return;
	}

	FVector CameraLocation = CamManager->GetCameraLocation();
	FVector CameraForward = CamManager->GetCameraRotation().Vector();
	CameraForward.Z = 0;
	FVector CameraForwardNormalized = CameraForward;
	CameraForwardNormalized.Normalize();

	AActor* Owner = GetOwner();
	FVector PlayerLocation = Owner->GetActorLocation();
	FVector PlayerForward = Owner->GetActorForwardVector();

	if(bShowDegugLines)
	{
		DrawDebugLine(GetWorld(), PlayerLocation, PlayerLocation + CameraForward * 500, FColor::Red, false, 2.0);
	}

	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, Owner);
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = false;

	struct FAudioHit
	{
		AAudioActorBase* Actor;
		FVector Location;
	};

	TArray<FAudioHit> AudioHits[3];

	FVector LineVector = CameraForward.RotateAngleAxis(-LineCastFieldOfView / 2, FVector::UpVector);

	const float AngleStep = LineCastFieldOfView / (LineCastCount * LineCastArcCount);
	for (int i = 0; i < LineCastArcCount; ++i)
	{
		for (int j = 0; j < LineCastCount; ++j)
		{
			LineVector = LineVector.RotateAngleAxis(AngleStep, FVector::UpVector);
			FVector End = PlayerLocation + LineVector * LineCastDistance;

			FHitResult HitResult(ForceInit);
			if (GetWorld()->LineTraceSingleByObjectType(HitResult, PlayerLocation, End, ECC_WorldStatic, CollisionParams))
			{
				FVector HitLocation = HitResult.ImpactPoint;
				class AAudioActorBase* AudioActor = Cast<AAudioActorBase>(HitResult.GetActor());
				if (AudioActor)
				{
					AudioHits[i].Add(FAudioHit{ AudioActor, HitLocation });
				}
			}
		}
	}

	// Get closest audio actor in the center FOV.

	AAudioActorBase* ClosestCenterAudioActor = nullptr;
	FVector ClosestCenterVector = FVector(LineCastDistance, LineCastDistance, LineCastDistance);

	for (FAudioHit hit : AudioHits[1])
	{
		if (FVector::Dist(PlayerLocation, hit.Location) < FVector::Dist(PlayerLocation, ClosestCenterVector))
		{
			ClosestCenterAudioActor = hit.Actor;
			ClosestCenterVector = hit.Location;
		}
	}

	if (ClosestCenterAudioActor)
	{
		ClosestCenterAudioActor->OnSoundEffectTriggered(FVector::Dist(PlayerLocation, ClosestCenterVector) / SpeedOfSound, ClosestCenterVector);
		if (bShowDegugLines)
		{
			DrawDebugSphere(GetWorld(), ClosestCenterVector, 10, 8, FColor::Red, false, 2.0);
		}
	}

	FVector ClosestCenterVectorNormalized = (ClosestCenterVector - PlayerLocation);
	ClosestCenterVectorNormalized.Normalize();
	float ClosestCenterAngle = FMath::Fmod(FMath::Atan2(ClosestCenterVectorNormalized.Y, ClosestCenterVectorNormalized.X) + (PI * 2), PI * 2);

	// Get the closest audio actor on the sides that are outside some angle threshold.

	for (int i = 0; i < LineCastArcCount; i += 2)
	{
		AAudioActorBase* ClosestLRAudioActor = nullptr;
		FVector ClosestLRVector = FVector(LineCastDistance, LineCastDistance, LineCastDistance);

		for (FAudioHit hit : AudioHits[i])
		{
			bool BeatsThreshold = true;
			if (ClosestCenterAudioActor)
			{
				FVector HitVectorNormalized = (hit.Location - PlayerLocation);
				HitVectorNormalized.Normalize();
				float LRAngle = FMath::Fmod(FMath::Atan2(HitVectorNormalized.Y, HitVectorNormalized.X) + (PI * 2), PI * 2);
				float AngleBetween = FMath::Abs(LRAngle - ClosestCenterAngle);
				BeatsThreshold = AngleBetween > FMath::DegreesToRadians(LineCastAngleThresholdForSound) && FVector::Dist(ClosestCenterVector, hit.Location) > LineCastDistanceThresholdForSound;
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Angle: %f > %f"), AngleBetween, FMath::DegreesToRadians(LineCastAngleThresholdForSound)));
			}

			if (BeatsThreshold)
			{
				if (FVector::Dist(PlayerLocation, hit.Location) < FVector::Dist(PlayerLocation, ClosestLRVector))
				{
					ClosestLRAudioActor = hit.Actor;
					ClosestLRVector = hit.Location;
				}
			}
		}

		if (ClosestLRAudioActor)
		{
			ClosestLRAudioActor->OnSoundEffectTriggered(FVector::Dist(PlayerLocation, ClosestLRVector) / SpeedOfSound, ClosestLRVector);
			if (bShowDegugLines)
			{
				DrawDebugSphere(GetWorld(), ClosestLRVector, 10, 8, FColor::Red, false, 2.0);
			}
		}
	}
}

