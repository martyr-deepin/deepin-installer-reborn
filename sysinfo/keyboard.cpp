// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/keyboard.h"

#include <QDebug>
#include <QDomDocument>

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

const char kSetXkbMapCmd[] = "/usr/bin/setxkbmap";

const char kXkbBaseRule[] = "/usr/share/X11/xkb/rules/base.xml";
const char kXkbExtraRule[] = "/usr/share/X11/xkb/rules/base.extras.xml";

// Read modelList node.
void ReadModelList(const QDomNode& root, XkbModelList& model_list) {
  const QDomNodeList model_nodes(root.childNodes());
  for (int index = 0; index < model_nodes.length(); index++) {
    XkbModel xkb_model;
    const QDomNodeList config_nodes(model_nodes.at(index).childNodes());
    if (config_nodes.length() == 1) {
      // Read modelList / model / configItem.
      const QDomNodeList props(config_nodes.at(0).childNodes());
      for (int props_idx = 0; props_idx < props.length(); props_idx++) {
        const QDomNode prop(props.at(props_idx));
        if (prop.nodeName() == "name") {
          xkb_model.name = prop.nodeValue();
        } else if (prop.nodeName() == "description") {
          xkb_model.description = prop.nodeValue();
        } else if (prop.nodeName() == "vendor") {
          xkb_model.vendor = prop.nodeValue();
        }
      }
    }
    model_list.append(xkb_model);
  }
}

// Read configItem in layoutVariant node.
void ReadLayoutVariantList(const QDomNode& root,
                                 XKbLayoutVariantList& variant_list) {
  const QDomNodeList variant_nodes(root.childNodes());
  for (int variant_idx = 0; variant_idx < variant_nodes.length();
       variant_idx++) {
    const QDomNode variant_node(variant_nodes.at(variant_idx));
    XkbLayoutVariant layout_variant;
    const QDomNodeList props(variant_node.childNodes());
    for (int prop_idx = 0; prop_idx < props.length(); prop_idx++) {
      const QDomNode prop(props.at(prop_idx));
      if (prop.nodeName() == "name") {
        layout_variant.name = prop.nodeValue();
      } else if (prop.nodeName() == "description") {
        layout_variant.description = prop.nodeValue();
      } else if (prop.nodeName() == "shortDescription") {
        layout_variant.short_description = prop.nodeValue();
      } else if (prop.nodeName() == "languageList") {
        // Read layout / configItem / languageList
        const QDomNodeList lang_nodes(prop.childNodes());
        for (int lang_idx = 0; lang_idx < lang_nodes.length(); lang_idx++) {
          const QDomNode lang_node(lang_nodes.at(lang_idx));
          layout_variant.language_list.append(lang_node.nodeValue());
        }
      }
    }
    variant_list.append(layout_variant);
  }
}

// Read configItem in layout node.
void ReadLayoutConfigItem(const QDomNode& root, XkbLayout& xkb_layout) {
  const QDomNodeList props(root.childNodes());
  for (int prop_idx = 0; prop_idx < props.length(); prop_idx++) {
    const QDomNode prop(props.at(prop_idx));
    if (prop.nodeName() == "name") {
      xkb_layout.name = prop.nodeValue();
    } else if (prop.nodeName() == "description") {
      xkb_layout.description = prop.nodeValue();
    } else if (prop.nodeName() == "shortDescription") {
      xkb_layout.short_description = prop.nodeValue();
    } else if (prop.nodeName() == "languageList") {
      // Read layout / configItem / languageList
      const QDomNodeList lang_nodes(prop.childNodes());
      for (int lang_idx = 0; lang_idx < lang_nodes.length(); lang_idx++) {
        const QDomNode lang_node(lang_nodes.at(lang_idx));
        xkb_layout.language_list.append(lang_node.nodeValue());
      }
    }
  }
}

// Read layoutList node.
void ReadLayoutList(const QDomNode& root, XkbLayoutList& layout_list) {
  const QDomNodeList layout_nodes(root.childNodes());
  for (int layout_idx = 0; layout_idx < layout_nodes.length(); layout_idx++) {
    XkbLayout xkb_layout;
    const QDomNodeList content_nodes(layout_nodes.at(layout_idx).childNodes());
    for (int content_idx = 0; content_idx < content_nodes.length();
         content_idx++) {
      const QDomNode content_node(content_nodes.at(content_idx));
      if (content_node.nodeName() == "configItem") {
        // Read layout / configItem
        ReadLayoutConfigItem(content_node, xkb_layout);

      } else if (content_node.nodeName() == "variantList") {
        // Read layout / variantList
        ReadLayoutVariantList(content_node, xkb_layout.variant_list);
      }
    }
    layout_list.append(xkb_layout);
  }
}

// Read xkb config file at |filepath|. Returns false if failed.
bool ReadConfig(const QString& filepath, XkbConfig& config) {
  QDomDocument doc;
  if (!doc.setContent(ReadFile(filepath), true)) {
    return false;
  }

  const QDomElement root = doc.documentElement();
  const QDomNodeList nodes(root.childNodes());

  for (int index = 0; index < nodes.length(); index++) {
    const QDomNode node = nodes.at(index);
    if (node.nodeName() == "modelList") {
      // Read model list.
      ReadModelList(node, config.model_list);
    } else if (node.nodeName() == "layoutList") {
      // Read layout list.
      ReadLayoutList(node, config.layout_list);
    }
  }

  return true;
}

}  // namespace

XkbConfig ReadXkbConfig() {
  XkbConfig config;
  if (!ReadConfig(kXkbBaseRule, config)) {
    qWarning() << "Failed to read xkb config file" << kXkbBaseRule;
    return config;
  }

  if (!ReadConfig(kXkbExtraRule, config)) {
    qWarning() << "Failed to read xkb config file" << kXkbExtraRule;
    return config;
  }
  return config;
}

bool SetXkbLayout(const QString& layout) {
  return SpawnCmd(kSetXkbMapCmd, {layout});
}

bool SetXkbLayout(const QString& layout, const QString& variant) {
  return SpawnCmd(kSetXkbMapCmd, {layout, variant});
}

bool SetXkbModel(const QString& model) {
  return SpawnCmd(kSetXkbMapCmd, {"-model", model});
}

}  // namespace installer