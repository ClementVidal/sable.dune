rollout MainRollout "RenameObjects" width:170 height:80
(
	button RenameBtn "Rename" pos:[10,40] width:150 height:30
	checkbox SelectedObjectsCheck "Selected objects only" pos:[10,10] width:150 height:20
	
	on RenameBtn pressed do
	(
	
		if SelectedObjectsCheck.checked then
		(
			objSet = selection as array
		)
		else
		(
			objSet = objects as array
		)
		
		for obj in objSet do
		(
			objName = obj.name
			obj.mat.name = objName
		)
	)
)

macroScript RenameObject category:"Sable" tooltip:"RenameObject"
(
	on execute do
	(
		floater = newRolloutFloater "RenameObject" 200 200
		
		addRollout MainRollout floater
	)
)


