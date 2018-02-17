extends Node

const NameGenerator = preload("res://bin/namegen.gdns")

onready var name_gen = NameGenerator.new()

func _ready():
	var text = """aaron ababa abigail abrahan absalon abundio adalberto adan adela adelaida adelia adisoda adolfo adrian adriana adriano africa agapito agueda agustina aida aimar ainara ainhoa aitor alba albano alberto alejandra alejandro alfonso alfredo alicia alma almudena alodia alondra altagracia alvara alvaro amalia amanda amaya amelia amparo ana andrea andres angel angela angeles angelica anibal aniceto anita antero antonia antonieta antonio araceli arantxa ariel armando arturo ascension asuncion augustin aurelia aurelio aurora azucena barbara bartolome basilio beatriz begona belen beltran beneharo benita benito benjamin bernardino bernardo berta blanca blas bonifacio braulio brunilda calixto camila candida candido canuto caridad carla carlos carlota carmen carolina casimiro catalina catrina cayetano cecilia celia cesar chaxiraxi chema citlali clara claudia claudio clotilde cobura concepcion consolacion consuelo cornelio covadonga cristina cristobal cruz cruz dalila damaso damian daniel daniela daritza david dayana debora delia derque desamparados diana diego dionisia dionisio dolores dominga domingo dorotea echedey edgar edgardo edmundo eduardo efren egidio elena elias elisa eliseo eloy elvira emanuel emelda emilia emilio encarnacion enka enrique enriqueta ernestina ernesto esperanza estanislao esteban estefania estela ester esther eufemia eugenia eugenio eulalia eva ezequiel fabian fabiana fabio fabricia fabricio facunda facundo fadrique federico feliciana feliciano felicidad felipa felipe felisa fermin fermina fernan fernanda fernando ferrando fidel filomena firjas flavia flora florencia floria florio floro francisco franco frida froila froilan froilana fulberta fulberto fulca fulco gabriel gabriela gara gema genaro german gershu gilberto ginebra gines gonzalo graciela gregorio guadalupe guadalupe guillermina guillermo gustavo hanagua haydee hector hernando hilda hipolito hortensia hugo humberto ignacio ilda imelda inaki ines inigo inmaculada irene ireneo isaac isabel isidoro isidro ismael itahisa ivan jacinta jacinto jacobo jacqueline jaime jairo javier javiera jesus jimena joaquin joaquina jonay jorge jorgelina jose josefa josefina josue juan juana juanca juanfran juano julian juliana julieta julio justin justo karina kebehi ladislao laura lea leandro leire leon leonardo leonor leopoldo leticia lia lidia lilia liliana lope lorena lorenza lorenzo lourdes lucas lucia luciana luciano luis luisa luna luz macarena magdalena maite manuel manuela marcela marcelina marcelino marcelo marcial marcos margarita maria mariana mariano maribel marina mario marta martin martina mateo matias matilde maximo mayte meagens mercedes micaela miguel miguela milagros mireia mirella mireya modesta modesto moises monica montserrat murillo narcisa narciso natalia natalio natan natan natividad nerea nicolas nicolau nieves nilda noelia noemi nuria ofelia olga ordono oriol oscar oswaldo pablito pablo paqui pascual pascuala patricia patricio paula paulette paulina paulito paz pedro pelayo piedad pilar pio purificacion rafael rafaela raimunda raimundo ramira ramiro ramon ramona raquel raul rebeca remedios renato ricarda ricardo roberto rocio rodolfa rodolfo rodrigo rogelio rosa rosalia rosario roxana ruben rut ruth ruy salomon salvador samuel sancho sandra santi santiago sara saul sebastian segundo selena serafina sergio silvia silvio simon sofia soledad sonia soraya susana tadeo tania teodora teodoro teofilo teresa thiare tiare timoteo tito tomas tomasa triana trinidad tristan trysta ulises ulrica unai ursula valentin valentina vane vanesa veronica vicenta vicente victoria violeta virginia vito ximena yago yesenia yolanda zulma"""
	$TrainTextEdit.text = text
	$LabelMax2.text = str($HSliderMax.value)
	$LabelMin2.text = str($HSliderMin.value)
	update_trained_msg()

func _on_TrainButton_pressed():
	# we need a PoolStringArray
	var names = $TrainTextEdit.text.split(" ")
	name_gen.train(names)
	update_trained_msg()

func update_trained_msg():
	$TrainedLabel.text = "TRAINED GENERATOR" if name_gen.is_trained() \
else "UNTRAINED GENERATOR"

func _on_GenerateButton_pressed():
	#name_gen.new_word(3, 8) # Single word
	var n = $NunWords.value
	var vMin = $HSliderMin.value
	var vMax = $HSliderMax.value
	$ResTextEdit.text = name_gen.new_words(n, vMin, vMax).join("\n")

func _on_HSliderMax_value_changed( value ):
	var minVal = $HSliderMin.value
	var maxVal = $HSliderMax.value
	if maxVal < minVal:
		$HSliderMax.value = minVal
	$LabelMax2.text = str($HSliderMax.value)

func _on_HSliderMin_value_changed( value ):
	var minVal = $HSliderMin.value
	var maxVal = $HSliderMax.value
	if maxVal < minVal:
		$HSliderMin.value = maxVal
	$LabelMin2.text = str($HSliderMin.value)
