#pragma once


struct Doodad {
	static int auto_increment;
	int unique_id;
	std::string id;
	int variation;
	glm::vec3 position;
	glm::vec3 scale;
	float angle;

	enum class State {
		invisible_non_solid,
		visible_non_solid,
		visible_solid
	};
	State state;
	uint8_t life;

	int item_table_pointer;
	std::vector<ItemSet> item_sets;

	int world_editor_id;

	glm::mat4 matrix = glm::mat4(1.f);
	std::shared_ptr<StaticMesh> mesh;

	Doodad() {
		unique_id = auto_increment++;
	}
};

struct SpecialDoodad {
	std::string id;
	int variation;
	glm::vec3 position;
	glm::mat4 matrix = glm::mat4(1.f);
	std::shared_ptr<StaticMesh> mesh;
};

class Doodads {
	std::vector<Doodad> doodads;
	std::vector<SpecialDoodad> special_doodads;
	
	std::unordered_map<std::string, std::shared_ptr<StaticMesh>> id_to_mesh;

	std::shared_ptr<Shader> shader;

	static constexpr int write_version = 8;
	static constexpr int write_subversion = 11;
	static constexpr int write_special_version = 0;
public:
	slk::SLK doodads_slk;
	slk::SLK doodads_meta_slk;
	slk::SLK destructibles_slk;
	slk::SLK destructibles_meta_slk;

	//QuadTree<Doodad> tree;

	bool load(BinaryReader& reader, Terrain& terrain);
	void save() const;
	void load_destructible_modifications(BinaryReader& reader);
	void load_doodad_modifications(BinaryReader& reader);
	void update_area(const QRect& area);
	void create();
	void render();

	Doodad& add_doodad(std::string id, glm::vec2 position);
	void remove_doodad(Doodad* doodad);

	std::vector<Doodad*> query_area(QRectF area);
	void remove_doodads(const std::vector<Doodad*> list);

	std::shared_ptr<StaticMesh> get_mesh(std::string id, int variation);
};