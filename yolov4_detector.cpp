#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void detectPeople(const cv::Mat& frame, cv::dnn::Net& net, int frameIndex, int skipFrames, const std::string& outputDir, const std::string& videoName) {
    // Redimensiona e normaliza o frame para YOLOv4
    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);

    net.setInput(blob);

    // Executa a inferência
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    bool personDetected = false;

    for (const auto& detection : outputs) {
        const float* data = (float*)detection.data;
        for (size_t i = 0; i < detection.rows; ++i, data += detection.cols) {
            float confidence = data[4];  // Confiança da detecção
            if (confidence > 0.5) {  // Ajuste do limiar de confiança
                int classId = std::max_element(data + 5, data + detection.cols) - (data + 5);
                if (classId == 0 || classId == 15 || classId == 16) {  // YOLOv4: classe 0 é "person"
                    personDetected = true;
                    break;
                }
            }
        }
        if (personDetected) break;
    }

    // Salva o frame se uma pessoa foi detectada
    if (personDetected && frameIndex % skipFrames == 0) {
        // Cria o nome do arquivo com o prefixo do nome do vídeo
        std::string outputFile = outputDir + "/" + videoName + "_frame_" + std::to_string(frameIndex) + ".jpg";
        if (cv::imwrite(outputFile, frame)) {
        std::cout << "Frame salvo: " << outputFile << std::endl;
        } else {
            std::cerr << "Erro ao salvar frame: " << outputFile << std::endl;
        }
    }
}

int main() {
    std::string videoDir = "./CAMERA/";  // Diretório onde os vídeos estão localizados
    std::string modelConfig = "./settings/yolov4.cfg";   // Caminho para o arquivo .cfg do YOLOv4
    std::string modelWeights = "./settings/yolov4.weights";  // Caminho para o arquivo .weights do YOLOv4
    std::string outputDir = "./output/";

    // Cria o diretório de saída se não existir
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Carrega o modelo YOLOv4
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);

    // Itera sobre todos os arquivos no diretório de vídeos
    for (const auto& entry : fs::directory_iterator(videoDir)) {
        if (entry.is_regular_file() && (entry.path().extension() == ".mp4" || entry.path().extension() == ".avi" || entry.path().extension() == ".mkv")) {
            std::string videoPath = entry.path().string();
            std::string videoName = entry.path().stem().string();  // Nome do vídeo sem extensão

            std::cout << "Processando vídeo: " << videoPath << std::endl;
            
            // Captura o vídeo
            cv::VideoCapture cap(videoPath);
            if (!cap.isOpened()) {
                std::cerr << "Erro ao abrir o vídeo: " << videoPath << std::endl;
                continue;
            }

            int frameIndex = 0;
            int skipFrames = 60;

            cv::Mat frame;
            while (cap.read(frame)) {
                detectPeople(frame, net, frameIndex, skipFrames, outputDir, videoName);
                frameIndex++;
            }

            std::cout << "Processamento concluído para o vídeo: " << videoName << std::endl;
        }
    }

    std::cout << "Processamento concluído para todos os vídeos. Frames salvos em: " << outputDir << std::endl;
    return 0;
}
