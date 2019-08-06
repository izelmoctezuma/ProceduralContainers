// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include "ProceduralContainer.generated.h"

UCLASS()
class CONTAINERS_API AProceduralContainer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralContainer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent);

	UStaticMeshComponent* CreateSubComponent(UStaticMesh* Mesh, FVector Location, FRotator Rotation);

	bool IsColliding(UStaticMeshComponent* CurrentItem);

	void Populate();

//	UPROPERTY(VisibleAnywhere)
//		UBoxComponent* BoundingBox;

	UPROPERTY(EditAnywhere, Category = "Container Parameters")
		TArray<UStaticMesh*> MeshArray;

	UPROPERTY(EditAnywhere, Category = "Container Parameters")
		float ContainerExtent;

	UPROPERTY(EditAnywhere, Category = "Container Parameters")
		bool RandomizeRot;
	
};
