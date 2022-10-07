// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorSwitch.generated.h"

UCLASS()
class AKTOR_API ADoorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorSwitch();

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* SwitchMesh;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DoorMesh;
	
	UPROPERTY(VisibleAnywhere)
	class UTimelineComponent* Transition;
	UPROPERTY(VisibleAnywhere)
	class UCurveFloat* SwitchCurve;
	UPROPERTY(VisibleAnywhere)
	class UCurveFloat* DoorCurve;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FVector SwitchInitialLocation;
	FVector DoorInitialLocation;

	UFUNCTION()
		void UpdateSwitchZOffset(float Value);
	UFUNCTION()
		void UpdateDoorZOffset(float Value);
	UFUNCTION()
		void FinishedFunc();
};
