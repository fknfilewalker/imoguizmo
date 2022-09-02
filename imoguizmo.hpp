// +----------------------------------------------------------------------
// | Project : imoguizmo.
// | All rights reserved.
// +----------------------------------------------------------------------
// | Copyright (c) Lukas Lipp 2022.
// +----------------------------------------------------------------------
// | * Redistribution and use of this software in source and binary forms,
// |   with or without modification, are permitted provided that the following
// |   conditions are met:
// |
// | * Redistributions of source code must retain the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer.
// |
// | * Redistributions in binary form must reproduce the above
// |   copyright notice, this list of conditions and the
// |   following disclaimer in the documentation and/or other
// |   materials provided with the distribution.
// |
// | * Neither the name of the ray team, nor the names of its
// |   contributors may be used to endorse or promote products
// |   derived from this software without specific prior
// |   written permission of the ray team.
// |
// | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// | "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// | LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// | A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// | OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// | SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// | LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// | DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// | THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// | (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// | OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// +----------------------------------------------------------------------

#pragma once
#include <imgui.h>
#include <cmath>
#include <vector>
#include <algorithm>

namespace ImOGuizmo {
	struct ImVec3
	{
		ImVec3(const float x, const float y, const float z) : mData{ x, y, z } {}
		explicit ImVec3(const float* const data) : mData{ data[0], data[1], data[2] } {}
		float operator[](const int idx) const { return mData[idx]; }
		ImVec3 operator+(const ImVec3& other) const { return { mData[0] + other[0], mData[1] + other[1], mData[2] + other[2] }; }
		ImVec3 operator-(const ImVec3& other) const { return { mData[0] - other[0], mData[1] - other[1], mData[2] - other[2] }; }
		ImVec3 operator*(const float scalar) const { return { mData[0] * scalar, mData[1] * scalar, mData[2] * scalar }; }
		ImVec3 operator*(const ImVec3& other) const { return { mData[0] * other[0], mData[1] * other[1], mData[2] * other[2] }; }
		float mData[3];
	};
	inline ImVec4 multiply(const float* const m, const ImVec4& v)
	{
		const float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
		const float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
		const float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
		const float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
		return { x, y, z, w };
	}
	inline void multiply(const float* const l, const float* const r, float* out)
	{
		out[0] = l[0] * r[0] + l[1] * r[4] + l[2] * r[8] + l[3] * r[12];
		out[1] = l[0] * r[1] + l[1] * r[5] + l[2] * r[9] + l[3] * r[13];
		out[2] = l[0] * r[2] + l[1] * r[6] + l[2] * r[10] + l[3] * r[14];
		out[3] = l[0] * r[3] + l[1] * r[7] + l[2] * r[11] + l[3] * r[15];

		out[4] = l[4] * r[0] + l[5] * r[4] + l[6] * r[8] + l[7] * r[12];
		out[5] = l[4] * r[1] + l[5] * r[5] + l[6] * r[9] + l[7] * r[13];
		out[6] = l[4] * r[2] + l[5] * r[6] + l[6] * r[10] + l[7] * r[14];
		out[7] = l[4] * r[3] + l[5] * r[7] + l[6] * r[11] + l[7] * r[15];

		out[8] = l[8] * r[0] + l[9] * r[4] + l[10] * r[8] + l[11] * r[12];
		out[9] = l[8] * r[1] + l[9] * r[5] + l[10] * r[9] + l[11] * r[13];
		out[10] = l[8] * r[2] + l[9] * r[6] + l[10] * r[10] + l[11] * r[14];
		out[11] = l[8] * r[3] + l[9] * r[7] + l[10] * r[11] + l[11] * r[15];

		out[12] = l[12] * r[0] + l[13] * r[4] + l[14] * r[8] + l[15] * r[12];
		out[13] = l[12] * r[1] + l[13] * r[5] + l[14] * r[9] + l[15] * r[13];
		out[14] = l[12] * r[2] + l[13] * r[6] + l[14] * r[10] + l[15] * r[14];
		out[15] = l[12] * r[3] + l[13] * r[7] + l[14] * r[11] + l[15] * r[15];
	}

	inline bool checkInsideCircle(const ImVec2 center, const float radius, const ImVec2 point)
	{
		return (point.x - center.x) * (point.x - center.x) + (point.y - center.y) * (point.y - center.y) <= radius * radius;
	}

	inline void drawPositiveLine(const ImVec2 center, const ImVec2 axis, const ImU32 color, const float radius, const float thickness, const char* text, const bool selected) {
		const auto lineEndPositive = ImVec2(center.x + axis.x, center.y + axis.y);
		ImGui::GetWindowDrawList()->AddLine(center, lineEndPositive, color, thickness);
		ImGui::GetWindowDrawList()->AddCircleFilled(lineEndPositive, radius, color);
		const auto textPosX = ImVec2(floor(lineEndPositive.x - 3.0f), floor(lineEndPositive.y - 6.0f));
		if (selected) {
			ImGui::GetWindowDrawList()->AddCircle(lineEndPositive, radius, IM_COL32_WHITE, 0, 1.1f);
			ImGui::GetWindowDrawList()->AddText(textPosX, IM_COL32_WHITE, text);
		}
		else ImGui::GetWindowDrawList()->AddText(textPosX, IM_COL32_BLACK, text);
	}
	inline void drawNegativeLine(const ImVec2 center, const ImVec2 axis, const ImU32 color, const float radius, const bool selected) {
		const auto lineEndNegative = ImVec2(center.x - axis.x, center.y - axis.y);
		ImGui::GetWindowDrawList()->AddCircleFilled(lineEndNegative, radius, color);
		if (selected) {
			ImGui::GetWindowDrawList()->AddCircle(lineEndNegative, radius, IM_COL32_WHITE, 0, 1.1f);
		}
	}

	inline void buildPerspectiveProjMatrix(float* projMatrix, const float fovy, const float aspect, const float zNear, const float zFar) {
		std::memset(projMatrix, 0, sizeof(float) * 16);
		const float tanHalfFovy = tan(fovy / static_cast<float>(2));
		projMatrix[0] = 1.0f / (aspect * tanHalfFovy);
		projMatrix[5] = 1.0f / (tanHalfFovy);
		projMatrix[10] = zFar / (zNear - zFar);
		projMatrix[11] = -1.0f;
		projMatrix[14] = -(zFar * zNear) / (zFar - zNear);
	}

	inline void buildViewMatrix(float* viewMatrix, ImVec3 const& aPosition, ImVec3 const& right, ImVec3 const& up, ImVec3 const& forward) {
		// first column
		viewMatrix[0] = right[0];
		viewMatrix[4] = right[1];
		viewMatrix[8] = right[2];
		viewMatrix[12] = (-aPosition[0] * right[0] + -aPosition[1] * right[1] + -aPosition[2] * right[2]);
		// second column
		viewMatrix[1] = up[0];
		viewMatrix[5] = up[1];
		viewMatrix[9] = up[2];
		viewMatrix[13] = (-aPosition[0] * up[0] + -aPosition[1] * up[1] + -aPosition[2] * up[2]);
		// third column
		viewMatrix[2] = forward[0];
		viewMatrix[6] = forward[1];
		viewMatrix[10] = forward[2];
		viewMatrix[14] = (-aPosition[0] * forward[0] + -aPosition[1] * forward[1] + -aPosition[2] * forward[2]);
		// fourth column
		viewMatrix[3] = 0;
		viewMatrix[7] = 0;
		viewMatrix[11] = 0;
		viewMatrix[15] = 1;
	}

	inline void invert4x4(const float* m, float* out)
	{
		out[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		out[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		out[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		out[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		out[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		out[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		out[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		out[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		out[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		out[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		out[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		out[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		out[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		out[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		out[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		out[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

		float det = m[0] * out[0] + m[1] * out[4] + m[2] * out[8] + m[3] * out[12];
		det = 1.0f / det;
		for (uint32_t i = 0; i < 16; i++) out[i] = out[i] * det;
	}

	inline bool drawGizmo(float* viewMatrix, const ImVec2 pos, const float size, const float pivotDistance = 0.0f, const bool background = false) {
		const float hSize = size * 0.5f;
		const auto center = ImVec2(pos.x + hSize, pos.y + hSize);
		ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize({ size, size });
		// mat
		// to check with glm proj
		//glm::mat4 m = glm::perspective(0.01745329251994329576923690768489f * 90.0f, 1.0f, 0.1f, 1000.0f);
		//float *ptr = glm::value_ptr(m);
		static const float projectionMatrix[16] = { 1.0f / (1 * tan((0.01745329251994329576923690768489f * 90.0f) / 2.0f)), 0, 0, 0,
										0, 1.0f / tan((0.01745329251994329576923690768489f * 90.0f) / 2.0f), 0, 0,
										0, 0, 1000.0f / (0.1f - 1000.0f), -1.0f,
										0, 0, -(1000.0f * 0.1f) / (1000.0f - 0.1f), 0 };
		//buildPerspectiveProjMatrix(projectionMatrix, 0.01745329251994329576923690768489f * 90.0f, 1.0f, 0.1f, 1000.0f);

		float viewProjection[16];
		multiply(viewMatrix, projectionMatrix, viewProjection);
		// axis
		const float axisLength = size / 3.0f;
		const ImVec4 xAxis = multiply(viewProjection, ImVec4(axisLength, 0, 0, 0));
		const ImVec4 yAxis = multiply(viewProjection, ImVec4(0, axisLength, 0, 0));
		const ImVec4 zAxis = multiply(viewProjection, ImVec4(0, 0, axisLength, 0));

		const bool interactive = pivotDistance > 0.0f;
		const ImVec2 mousePos = ImGui::GetIO().MousePos;
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
		if (!background) flags |= ImGuiWindowFlags_NoBackground;
		const float radius = hSize * 0.88f;
		if (!interactive || !checkInsideCircle(center, radius, mousePos)) flags |= ImGuiWindowFlags_NoInputs;

		if (ImGui::Begin("ImOGuizmo", nullptr, flags))
		{
			const bool hover = ImGui::IsWindowHovered();
			if (interactive && hover) ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, IM_COL32(100, 100, 100, 130));

			const float positiveRadius = size * 0.075f;	// for size 120 -> 9;
			const float negativeRadius = size * 0.05f;	// for size 120 -> 6;
			const bool redPositiveCloser = 0.0f >= xAxis.w;
			const bool greenPositiveCloser = 0.0f >= yAxis.w;
			const bool bluePositiveCloser = 0.0f >= zAxis.w;

			constexpr ImU32 redFrontColor = IM_COL32(255, 54, 83, 255);
			constexpr ImU32 redBackColor = IM_COL32(154, 57, 71, 255);
			constexpr ImU32 greenFrontColor = IM_COL32(138, 219, 0, 255);
			constexpr ImU32 greenBackColor = IM_COL32(98, 138, 34, 255);
			constexpr ImU32 blueFrontColor = IM_COL32(44, 143, 255, 255);
			constexpr ImU32 blueBackColor = IM_COL32(52, 100, 154, 255);

			// sort axis based on distance
			// 0 : +x axis, 1 : +y axis, 2 : +z axis, 3 : -x axis, 4 : -y axis, 5 : -z axis
			std::vector<std::pair<int, float>> pairs = { {0, xAxis.w}, {1, yAxis.w}, {2, zAxis.w}, {3, -xAxis.w}, {4, -yAxis.w}, {5, -zAxis.w} };
			sort(pairs.begin(), pairs.end(), [=](const std::pair<int, float>& aA, const std::pair<int, float>& aB) { return aA.second > aB.second; });

			// find selection, front to back
			int selection = -1;
			for (auto it = pairs.crbegin(); it != pairs.crend() && selection == -1 && interactive; ++it) {
				switch (it->first) {
				case 0: // +x axis
					if (checkInsideCircle(ImVec2(center.x + xAxis.x, center.y + xAxis.y), positiveRadius, mousePos)) selection = 0;
					break;
				case 1: // +y axis
					if (checkInsideCircle(ImVec2(center.x + yAxis.x, center.y + yAxis.y), positiveRadius, mousePos)) selection = 1;
					break;
				case 2: // +z axis
					if (checkInsideCircle(ImVec2(center.x + zAxis.x, center.y + zAxis.y), positiveRadius, mousePos)) selection = 2;
					break;
				case 3: // -x axis
					if (checkInsideCircle(ImVec2(center.x - xAxis.x, center.y - xAxis.y), negativeRadius, mousePos)) selection = 3;
					break;
				case 4: // -y axis
					if (checkInsideCircle(ImVec2(center.x - yAxis.x, center.y - yAxis.y), negativeRadius, mousePos)) selection = 4;
					break;
				case 5: // -z axis
					if (checkInsideCircle(ImVec2(center.x - zAxis.x, center.y - zAxis.y), negativeRadius, mousePos)) selection = 5;
					break;
				default: break;
				}
			}

			// draw back first
			const float lineThickness = size * 0.017f; // for size 120 -> 2.0f;
			for (const auto& [fst, snd] : pairs) {
				switch (fst) {
				case 0: // +x axis
					drawPositiveLine(center, ImVec2(xAxis.x, xAxis.y), redPositiveCloser ? redFrontColor : redBackColor, positiveRadius, lineThickness, "X", selection == 0);
					continue;
				case 1: // +y axis
					drawPositiveLine(center, ImVec2(yAxis.x, yAxis.y), greenPositiveCloser ? greenFrontColor : greenBackColor, positiveRadius, lineThickness, "Y", selection == 1);
					continue;
				case 2: // +z axis
					drawPositiveLine(center, ImVec2(zAxis.x, zAxis.y), bluePositiveCloser ? blueFrontColor : blueBackColor, positiveRadius, lineThickness, "Z", selection == 2);
					continue;
				case 3: // -x axis
					drawNegativeLine(center, ImVec2(xAxis.x, xAxis.y), !redPositiveCloser ? redFrontColor : redBackColor, negativeRadius, selection == 3);
					continue;
				case 4: // -y axis
					drawNegativeLine(center, ImVec2(yAxis.x, yAxis.y), !greenPositiveCloser ? greenFrontColor : greenBackColor, negativeRadius, selection == 4);
					continue;
				case 5: // -z axis
					drawNegativeLine(center, ImVec2(zAxis.x, zAxis.y), !bluePositiveCloser ? blueFrontColor : blueBackColor, negativeRadius, selection == 5);
					continue;
				default: break;
				}
			}
			ImGui::End();

			if (selection != -1 && ImGui::IsMouseClicked(ImGuiPopupFlags_MouseButtonLeft)) {
				float modelMat[16];
				invert4x4(viewMatrix, modelMat);
				const ImVec3 pivotPos = ImVec3(&modelMat[12]) - ImVec3(&modelMat[8]) * pivotDistance;
				// +x axis 
				if (selection == 0) buildViewMatrix(viewMatrix, pivotPos + ImVec3(pivotDistance, 0, 0), ImVec3(0, 0, -1), ImVec3(0, 1, 0), ImVec3(1, 0, 0));
				// +y axis 
				if (selection == 1) buildViewMatrix(viewMatrix, pivotPos + ImVec3(0, pivotDistance, 0), ImVec3(1, 0, 0), ImVec3(0, 0, -1), ImVec3(0, 1, 0));
				// +z axis 
				if (selection == 2) buildViewMatrix(viewMatrix, pivotPos + ImVec3(0, 0, pivotDistance), ImVec3(1, 0, 0), ImVec3(0, 1, 0), ImVec3(0, 0, 1));
				// -x axis 
				if (selection == 3) buildViewMatrix(viewMatrix, pivotPos - ImVec3(pivotDistance, 0, 0), ImVec3(0, 0, 1), ImVec3(0, 1, 0), ImVec3(-1, 0, 0));
				// -y axis 
				if (selection == 4) buildViewMatrix(viewMatrix, pivotPos - ImVec3(0, pivotDistance, 0), ImVec3(1, 0, 0), ImVec3(0, 0, 1), ImVec3(0, -1, 0));
				// -z axis 
				if (selection == 5) buildViewMatrix(viewMatrix, pivotPos - ImVec3(0, 0, pivotDistance), ImVec3(-1, 0, 0), ImVec3(0, 1, 0), ImVec3(0, 0, -1));

				return true;
			}
		}
		return false;
	}
}