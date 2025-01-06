#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void detectPeople(const cv::Mat& frame, cv::dnn::Net& net, int frameIndex, int skipFrames, const std::string& outputDir) {
    // Redimensiona e normaliza o frame para YOLOv8
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(640, 640), cv::Scalar(0, 0, 0), true, false);

    net.setInput(blob);

    // Executa a inferência
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    bool personDetected = false;

    // Agora a saída da rede tem a forma [1, 84, 8400], vamos percorrer as detecções
    const float* data = (float*)outputs[0].data;
    int numBoxes = 8400;
    int numClasses = 84;

    for (int i = 0; i < numBoxes; ++i) {
        // Calcular o índice do ponto de dados correspondente à caixa de detecção
        float confidence = data[i * (numClasses + 5) + 4];  // Confiança da detecção (5º valor de cada caixa)
        
        if (confidence > 0.5) {  // Ajuste do limiar de confiança
            int classId = std::max_element(data + i * (numClasses + 5) + 5, data + (i + 1) * (numClasses + 5)) - (data + i * (numClasses + 5) + 5);
            
            if (classId == 0) {  // YOLOv8: classe 0 é "person"
                personDetected = true;
                break;
            }
        }
    }

    // Salva o frame se uma pessoa foi detectada
    if (personDetected && frameIndex % skipFrames == 0) {
        std::string outputFile = outputDir + "/frame_" + std::to_string(frameIndex) + ".jpg";
        cv::imwrite(outputFile, frame);
        std::cout << "Frame salvo: " << outputFile << std::endl;
    }
}

int main() {
    std::string videoPath = "./CAMERA/output.mp4";
    std::string modelPath = "./settings/yolov8n.onnx";
    std::string outputDir = "./output/";

    std::cout << "Caminho do vídeo: " << videoPath << std::endl;

    // Cria o diretório de saída se não existir
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Carrega o modelo ONNX
    cv::dnn::Net net = cv::dnn::readNetFromONNX(modelPath);

    // Verifica se o modelo foi carregado corretamente
    if (net.empty()) {
        std::cerr << "Erro ao carregar o modelo YOLOv8!" << std::endl;
        return -1;
    }

    // Captura o vídeo
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Erro ao abrir o vídeo!" << std::endl;
        return -1;
    }

    int frameIndex = 0;
    int skipFrames = 50;  // Salvar a cada 30 frames

    cv::Mat frame;
    while (cap.read(frame)) {
        detectPeople(frame, net, frameIndex, skipFrames, outputDir);
        frameIndex++;
    }

    std::cout << "Processamento concluído. Frames salvos em: " << outputDir << std::endl;
    return 0;
}
