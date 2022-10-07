// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADoorSwitch::ADoorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	SwitchMesh->SetupAttachment(TriggerBox);
	SwitchMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(TriggerBox);
	DoorMesh->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
	DoorMesh->SetRelativeScale3D(FVector(0.25f, 2.5f, 4.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (StaticMeshAsset.Succeeded()) {
		SwitchMesh->SetStaticMesh(StaticMeshAsset.Object);
		DoorMesh->SetStaticMesh(StaticMeshAsset.Object);
	}

	Transition = CreateDefaultSubobject<UTimelineComponent>(TEXT("Transition"));

	static ConstructorHelpers::FObjectFinder<UCurveFloat> SwitchCurveAsset(TEXT("CurveFloat'/Game/Blueprint/SwitchCurve.SwitchCurve'"));
	if (SwitchCurveAsset.Succeeded()) {
		SwitchCurve = SwitchCurveAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> DoorCurveAsset(TEXT("CurveFloat'/Game/Blueprint/DoorCurve.DoorCurve'"));
	if (DoorCurveAsset.Succeeded()) {
		DoorCurve = DoorCurveAsset.Object;
	}
}

// Called when the game starts or when spawned
void ADoorSwitch::BeginPlay()
{
	Super::BeginPlay();

	SwitchInitialLocation = SwitchMesh->GetRelativeLocation();
	DoorInitialLocation = DoorMesh->GetRelativeLocation();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoorSwitch::OnOverlapEnd);

	if (SwitchCurve && DoorCurve) {
		FOnTimelineFloat UpdateBind;
		UpdateBind.BindDynamic(this, &ADoorSwitch::UpdateSwitchZOffset);
		Transition->AddInterpFloat(SwitchCurve, UpdateBind, FName("Alpha"), FName("SwitchCurve"));

		UpdateBind.Unbind();
		UpdateBind.BindDynamic(this, &ADoorSwitch::UpdateDoorZOffset);
		Transition->AddInterpFloat(DoorCurve, UpdateBind, FName("Beta"), FName("DoorCurve"));

		FOnTimelineEvent FinishedBind;
		FinishedBind.BindDynamic(this, &ADoorSwitch::FinishedFunc);
		Transition->SetTimelineFinishedFunc(FinishedBind);
	}
	//Transition->Play();    //update and finished
	//Transition->Reverse();    //update and finished
	//Transition->SetLooping(true);    //loops, but always starts from the beginning on each loop
	//Transition->SetIgnoreTimeDilation(true);    ////ignores world time, means ignores if world is slow motion.
}

// Called every frame
void ADoorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	Transition->Play();
}
void ADoorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	Transition->Reverse();
}

void ADoorSwitch::UpdateSwitchZOffset(float Value) {
	SwitchMesh->SetRelativeLocation(SwitchInitialLocation + FVector(0.f, 0.f, Value));
}
void ADoorSwitch::UpdateDoorZOffset(float Value) {
	DoorMesh->SetRelativeLocation(DoorInitialLocation + FVector(0.f, 0.f, Value));
}
void ADoorSwitch::FinishedFunc() {
	UE_LOG(LogTemp, Warning, TEXT("Finished"));
}