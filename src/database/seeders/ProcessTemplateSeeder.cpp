#include "ProcessTemplateSeeder.h"
#include "../../core/Logger.h"

ProcessTemplateSeeder::ProcessTemplateSeeder(ProcessTemplateRepository& repository)
    : processTemplateRepository(repository)
{
}

bool ProcessTemplateSeeder::shouldSeed()
{
    auto templates = processTemplateRepository.findAll();
    return templates.isEmpty();
}

bool ProcessTemplateSeeder::seed()
{
    LOG_INFO("Seeding process templates");

    auto processTemplates = getDefaultTemplates();
    int successCount = 0;

    for (const auto& processTemplate : processTemplates)
    {
        auto result = processTemplateRepository.save(processTemplate);
        if (result.has_value())
        {
            successCount++;
        }
        else
        {
            LOG_WARNING("Failed to save template: " + processTemplate.getName());
        }
    }

    LOG_INFO("Successfully seeded " + QString::number(successCount) + " out of " +
             QString::number(processTemplates.size()) + " templates");
    return successCount == processTemplates.size();
}

QString ProcessTemplateSeeder::getName()
{
    return "ProcessTemplateSeeder";
}

QVector<ProcessTemplate> ProcessTemplateSeeder::getDefaultTemplates()
{
    QVector<ProcessTemplate> templates;

    ProcessTemplate nodeFrontend;
    nodeFrontend.setName("Node.js Frontend");
    nodeFrontend.setCommand("npm start");
    nodeFrontend.setPort(3000);
    nodeFrontend.setDescription("React/Vue/Angular frontend application");
    nodeFrontend.setParameters("");
    nodeFrontend.setEnvironment("NODE_ENV=development");
    templates.append(nodeFrontend);

    ProcessTemplate nodeBackend;
    nodeBackend.setName("Node.js Backend");
    nodeBackend.setCommand("npm run dev");
    nodeBackend.setPort(3001);
    nodeBackend.setDescription("Node.js backend API server");
    nodeBackend.setParameters("");
    nodeBackend.setEnvironment("NODE_ENV=development\nPORT=3001");
    templates.append(nodeBackend);

    ProcessTemplate vue;
    vue.setName("Vue.js App");
    vue.setCommand("npm run serve");
    vue.setPort(8080);
    vue.setDescription("Vue.js frontend application");
    vue.setParameters("");
    vue.setEnvironment("");
    templates.append(vue);

    ProcessTemplate angular;
    angular.setName("Angular App");
    angular.setCommand("ng serve");
    angular.setPort(4200);
    angular.setDescription("Angular frontend application");
    angular.setParameters("--open=false");
    angular.setEnvironment("");
    templates.append(angular);

    ProcessTemplate spring;
    spring.setName("Spring Boot API");
    spring.setCommand("mvn spring-boot:run");
    spring.setPort(8080);
    spring.setDescription("Spring Boot REST API");
    spring.setParameters("");
    spring.setEnvironment("SPRING_PROFILES_ACTIVE=dev");
    templates.append(spring);

    ProcessTemplate flask;
    flask.setName("Python Flask");
    flask.setCommand("python app.py");
    flask.setPort(5000);
    flask.setDescription("Python Flask web application");
    flask.setParameters("");
    flask.setEnvironment("FLASK_ENV=development\nFLASK_DEBUG=1");
    templates.append(flask);

    ProcessTemplate docker;
    docker.setName("Docker Container");
    docker.setCommand("docker run");
    docker.setPort(8080);
    docker.setDescription("Docker container service");
    docker.setParameters("-p 8080:8080\n--name my-service");
    docker.setEnvironment("");
    templates.append(docker);

    ProcessTemplate compose;
    compose.setName("Docker Compose");
    compose.setCommand("docker-compose up");
    compose.setPort(8080);
    compose.setDescription("Docker Compose stack");
    compose.setParameters("--build");
    compose.setEnvironment("");
    templates.append(compose);

    return templates;
}
