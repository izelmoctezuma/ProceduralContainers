// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralContainer.h"

TArray<UStaticMeshComponent*> ObjectArray;

// Sets default values
AProceduralContainer::AProceduralContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UBillboardComponent* LeftBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Root"));
	LeftBillboard->bIsScreenSizeScaled = true;
	LeftBillboard->ScreenSize = 0.0015;
	RootComponent = LeftBillboard;
	ContainerExtent = 100;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	UStaticMesh* DefaultMesh = MeshAsset.Object;
	MeshArray.Add(DefaultMesh);
	ObjectArray = {};
	RandomizeRot = true;
}

// Called when the game starts or when spawned
void AProceduralContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AProceduralContainer::IsColliding(UStaticMeshComponent* CurrentItem)
{
	if (ObjectArray.Num() == 0) {
		return false;
	}
	else {
		for (int n = 0; n < ObjectArray.Num(); n++) {
			FCollisionQueryParams CollisionParams;
			if (CurrentItem->ComponentOverlapComponent(ObjectArray[n], ObjectArray[n]->GetComponentLocation(), ObjectArray[n]->GetComponentRotation(), CollisionParams)) { // (CurrentItem->IsOverlappingComponent(ObjectArray[n])) {
				return true;
			}
		}
		return false;
	}
}

void AProceduralContainer::Populate() {
	ObjectArray = {};
	UE_LOG(LogTemp, Log, TEXT("Here is a property: %f !"), ContainerExtent);
	float CurrentPosition = 0;
	int i = 0;
	// as long as we still have space
	while (CurrentPosition <= ContainerExtent) {
		// pick a random mesh
		UStaticMesh* CurrentMesh = MeshArray[rand() % MeshArray.Num()];
		// either pick a random rotation or set it to default
		FRotator Rotation = FRotator(0, rand() % 91, 0);
		if (RandomizeRot == false) {
			Rotation = FRotator(0, 0, 0);
		}
		// create the item currently being placed
		UStaticMeshComponent* CurrentItem = (CreateSubComponent(CurrentMesh, FVector(0, 0, 0), Rotation));
		// get item bounds and calculate x offset for current rotation (width/2)
		float XOffset = CurrentItem->CalcBounds(CurrentItem->GetComponentTransform()).BoxExtent[0] / 2;
		// calculate rightmost position the item can occupy
		float RightSafePosition = ContainerExtent - XOffset;
		CurrentPosition = XOffset;
		UE_LOG(LogTemp, Log, TEXT("LSP: %f !"), CurrentPosition);
		CurrentItem->SetRelativeLocation(FVector(CurrentPosition, 0, 0));
		while (IsColliding(CurrentItem)) {
			CurrentPosition += 1;
			if (CurrentPosition > RightSafePosition) {
				CurrentItem->UnregisterComponent();
				CurrentItem->DestroyComponent();
				CurrentPosition = ContainerExtent + 1;
				break;
			}
			else {
				CurrentItem->SetRelativeLocation(FVector(CurrentPosition, 0, 0));
			}
		}
		if (CurrentItem != NULL) {
			ObjectArray.Add(CurrentItem);
		}

		i++;
	}
}

// Called after any property is changed
#if WITH_EDITOR
void AProceduralContainer::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Populate();
	/* Another way to destroy components:
	if (ObjectArray[2] != NULL) {
		ObjectArray[2]->UnregisterComponent();
		ObjectArray[2]->DestroyComponent();
	} */
}
#endif

UStaticMeshComponent* AProceduralContainer::CreateSubComponent(UStaticMesh* Mesh, FVector Location, FRotator Rotation)
{
	UStaticMeshComponent* newComponent = NewObject<UStaticMeshComponent>(this);
	newComponent->SetStaticMesh(Mesh);
	newComponent->RegisterComponentWithWorld(GetWorld());
	newComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	AddOwnedComponent(newComponent);
	newComponent->SetRelativeLocation(Location);
	newComponent->SetRelativeRotation(Rotation);
	newComponent->Mobility = EComponentMobility::Static;


	return newComponent;
}
