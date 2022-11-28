// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EcholocationAbility.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType )
class SEATTLESLOWJAM2022_API UEcholocationAbility : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEcholocationAbility();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	int LineCastCount = 32;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float LineCastFieldOfView = 240.0;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float LineCastAngleThresholdForSound = 60.0;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float LineCastDistanceThresholdForSound = 100.0;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float LineCastDistance = 2000.0;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float SpeedOfSound = 1000.0;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	bool bShowDegugLines = false;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Functions")
	void OnEcholocationBegin();
};
