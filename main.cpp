#include <entt/entt.hpp>
#include <ranges>
#include <string>
#include <iostream>

struct Scene;
struct GameObject 
{
    GameObject(Scene&);
    GameObject(Scene&, entt::entity);

    template<typename Component, typename... Args>
    Component& addComponent(Args&&... args)
    {
        return registry.emplace<Component>(id, std::forward<Args>(args)...);
    }

    template<typename Component>
    Component& getComponent()
    {
        return registry.get<Component>(id);
    }

    Scene&          scene;
    entt::registry& registry;
    entt::entity    id;
};

struct Scene 
{
    Scene()                         = default;
    Scene(const Scene&)             = delete;
    Scene(Scene&&)                  = delete;
    auto operator=(const Scene&)    = delete;
    auto operator=(Scene&&)         = delete;

    GameObject      getGameObject(entt::entity id)
    {
        return GameObject(*this, id);
    }

    GameObject      newGameObject()
    {
        return GameObject(*this);
    }

    template<typename... Components>
    auto view()
    {
        return View<Components...>(*this, registry);
    }

    entt::registry  registry;    

    template<typename... Components>
    struct View 
    {
        View(Scene& s, entt::registry& r)
            : scene(s)
            , enttView(r.view<Components...>()) {}

        auto each()
        {
            return enttView.each()
                | std::views::transform([&]<typename Tuple>(Tuple&& tuple)
                {
                    return std::apply([&](auto id, auto&... rest)
                    {
                        return std::tuple_cat(std::make_tuple(scene.getGameObject(id)), std::tie(rest...));
                    }, tuple);
                });
        }

        auto workingEach()
        {
            return enttView.each();
        }

    private:
        using EnttView = decltype(registry.view<Components...>());
        Scene&      scene;
        EnttView    enttView;
    };
};

struct ComponentID
{
    int id;
};

struct ComponentName
{
    std::string name;
};

int main()
{
    Scene scene;
    for (int i = 0; i < 100; i++)
    {
        auto obj = scene.newGameObject();
        obj.addComponent<ComponentID>(i);
        obj.addComponent<ComponentName>("name");
    }

    // THIS WORKS
    for (auto [entt, id] : scene.view<ComponentID>().workingEach())
    {
        std::cout << id.id << " " ;
        std::cout << scene.getGameObject(entt).getComponent<ComponentName>().name << "\n";
    }

    // THIS DOES NOT
    for (auto [obj, id] : scene.view<ComponentID>().each())
    {
        std::cout << id.id << " " ;
        std::cout << obj.getComponent<ComponentName>().name << "\n";
    }
}

GameObject::GameObject(Scene& s, entt::entity id)
    : scene(s)
    , registry(s.registry)
    , id(id)
{
}

GameObject::GameObject(Scene& s)
    : scene(s)
    , registry(s.registry)
    , id(registry.create())
{
}


