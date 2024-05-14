#include "Headers/BuzzModels.h"

void BuzzModels::initBuzzModels(Shader & shader) {
    modelBuzzTorso.loadModel("../models/buzz/buzzlightyTorso.obj");
    modelBuzzTorso.setShader(&shader);
    modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
    modelBuzzHead.setShader(&shader);
    modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");
    modelBuzzLeftArm.setShader(&shader);
    modelBuzzRightArm.loadModel("../models/buzz/buzzlightyRightArm.obj");
    modelBuzzRightArm.setShader(&shader);
    modelBuzzLeftForeArm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");
    modelBuzzLeftForeArm.setShader(&shader);
    modelBuzzRightForeArm.loadModel("../models/buzz/buzzlightyRightForearm.obj");
    modelBuzzRightForeArm.setShader(&shader);
    modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");
    modelBuzzLeftHand.setShader(&shader);
    modelBuzzRightHand.loadModel("../models/buzz/buzzlightyRightHand.obj");
    modelBuzzRightHand.setShader(&shader);
    modelBuzzHip.loadModel("../models/buzz/buzzlightyHip.obj");
    modelBuzzHip.setShader(&shader);
    modelBuzzLeftThigh.loadModel("../models/buzz/buzzlightyLeftThigh.obj");
    modelBuzzLeftThigh.setShader(&shader);
    modelBuzzLeftCalf.loadModel("../models/buzz/buzzlightyLeftCalf.obj");
    modelBuzzLeftCalf.setShader(&shader);
    modelBuzzLeftFoot.loadModel("../models/buzz/buzzlightyLeftFoot.obj");
    modelBuzzLeftFoot.setShader(&shader);
    modelBuzzRightThigh.loadModel("../models/buzz/buzzlightyRightThigh.obj");
    modelBuzzRightThigh.setShader(&shader);
    modelBuzzRightCalf.loadModel("../models/buzz/buzzlightyRightCalf.obj");
    modelBuzzRightCalf.setShader(&shader);
    modelBuzzRightFoot.loadModel("../models/buzz/buzzlightyRightFoot.obj");
    modelBuzzRightFoot.setShader(&shader);
    modelBuzzLeftWing1.loadModel("../models/buzz/buzzlightyLeftWing1.obj");
    modelBuzzLeftWing1.setShader(&shader);
    modelBuzzLeftWing2.loadModel("../models/buzz/buzzlightyLeftWing2.obj");
    modelBuzzLeftWing2.setShader(&shader);
    modelBuzzRightWing1.loadModel("../models/buzz/buzzlightyRightWing1.obj");
    modelBuzzRightWing1.setShader(&shader);
    modelBuzzRightWing2.loadModel("../models/buzz/buzzlightyRightWing2.obj");
    modelBuzzRightWing2.setShader(&shader);
}
void BuzzModels::destruir(){
    modelBuzzHead.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzLeftForeArm.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzTorso.destroy();
	//Brazo derecho
	modelBuzzRightArm.destroy();
	modelBuzzRightForeArm.destroy();
	modelBuzzRightHand.destroy();
	//Cadera
	modelBuzzHip.destroy();
	//Pierna izquierda
	modelBuzzLeftThigh.destroy();
	modelBuzzLeftCalf.destroy();
	modelBuzzLeftFoot.destroy();
	//Pierna derecha
	modelBuzzRightThigh.destroy();
	modelBuzzRightCalf.destroy();
	modelBuzzRightFoot.destroy();
	//Ala izquiera
	modelBuzzLeftWing1.destroy();
	modelBuzzLeftWing2.destroy();
	//Ala derecha
	modelBuzzRightWing1.destroy();
	modelBuzzRightWing2.destroy();
}