#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", mLastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		static bool enabledFrameRender = false;
		ImGui::Checkbox("Frame Render", &enabledFrameRender);
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		mViewportWidht = ImGui::GetContentRegionAvail().x;
		mViewportHeight = ImGui::GetContentRegionAvail().y;

		if (mImage)
			ImGui::Image(mImage->GetDescriptorSet(), { (float)mImage->GetWidth(), (float)mImage->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();

		if(enabledFrameRender)
			Render();
	}

	void Render()
	{
		Timer timer;

		if (mImage == nullptr || mViewportWidht != mImage->GetWidth() || mViewportHeight != mImage->GetHeight())
		{
			mImage = std::make_shared<Image>(mViewportWidht, mViewportHeight, ImageFormat::RGBA);
			delete[] mImageData;
			mImageData = new uint32_t[mViewportWidht * mViewportHeight];
		}

		// Iteration throught all pixels
		for (uint32_t i = 0; i < mViewportWidht * mViewportHeight; i++)
		{
			mImageData[i] = Random::UInt();
			//Sets the alpha channel to 1. it's an RGBA format form right to left
			mImageData[i] |= 0xff000000;
		}

		mImage->SetData(mImageData);

		mLastRenderTime = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Image> mImage;
	uint32_t* mImageData = nullptr;
	uint32_t mViewportWidht = 0;
	uint32_t mViewportHeight = 0;

	float mLastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}