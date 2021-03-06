#include <memory>
#include <experimental/filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <OpenDiablo2.Game/D2Engine.h>
#include <CLI/CLI.hpp>
#include <StormLib.h>

int
main(
	int    argc,
	char** argv)
{
	spdlog::set_level(spdlog::level::trace);
	spdlog::set_pattern("[%^%l%$] %v");
	spdlog::info("OpenDiablo 2 has started");

	CLI::App app{"OpenDiablo2 - An open source re-implementation of Diablo 2."};

	OpenDiablo2::Common::D2EngineConfig engineConfig;
	engineConfig.BasePath = std::experimental::filesystem::current_path().string();

	app.add_option("-p,--path", engineConfig.BasePath, "The base path for Diablo 2");

	CLI11_PARSE(app, argc, argv);

	spdlog::info("Base file path is '" + engineConfig.BasePath + "'");

	// Sanity-check that files are where we expect them to be...
	auto testFilePath = engineConfig.BasePath + "/d2data.mpq";

	HANDLE mpq = nullptr;

	if (!SFileOpenArchive(("flat-file:" + testFilePath).c_str(), 0, STREAM_FLAG_READ_ONLY, &mpq)) {
		spdlog::error("Diablo 2 content files were not detected. Please make sure the base path is properly set!");
		exit(0);
	}
	SFileCloseFile(mpq);
	spdlog::info("Content files were located, starting engine.");

	// Off to the races we go!
	auto engine = std::make_shared<OpenDiablo2::Game::D2Engine>(engineConfig);
	engine->Run();

	return 0;
}


